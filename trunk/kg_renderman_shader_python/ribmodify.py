from __future__ import generators
import os, sys, string, exceptions
import re, copy, kgUtils

rexp_1 = re.compile(r"""
	(?:
	\[
	\s*
	(?P<num>
	[-0-9]+
	| [-0-9]*[.][0-9]+
	| [-0-9]*[.][0-9]+[eE][+-]?[0-9]+
	)
	\s*	
	\]
	)  #num
	""", re.VERBOSE)

rexp_2 = re.compile(r"""
	(?P<vector>
	\[
	\s*	
	(?:
	[-0-9]+
	| [-0-9]*[.][0-9]+
	| [-0-9]*[.][0-9]+[eE][+-]?[0-9]+
	)
	(?:
	\s+
	(?:
	[-0-9]+
	| [-0-9]*[.][0-9]+
	| [-0-9]*[.][0-9]+[eE][+-]?[0-9]+
	)
	)*
	\s*
	\]
	)
	""", re.VERBOSE)
rexp_3 = re.compile(r"""
	(?:
	\[
	\s*
	["]
	(?P<string>	
	[^"]+
	)
	["]
	\s*
	\]
	)
	""", re.VERBOSE)


def ribTypeCast(paramType, val):
	retVal = val
	if(paramType == 'color'):
		pass
	elif(paramType == 'vector'):
		pass
	elif(paramType == 'string'):
		pass
	else:		
		retVal = eval(paramType + '(' + repr(val) + ')')
	return retVal

def parseLine(line):
	global rexp_2
	s = rexp_2.split(line)
	t = []
	for i in s:
		if (rexp_2.match(i) == None and rexp_3.match(i) == None):
			map(t.append, filter(lambda x:x != '', i.split()))
		elif i != '':
			t.append(i)
	return t

class myException(exceptions.Exception):
	"my exception"


class Options(object):
	def __init__(self, **kwds):
		self.__dict__.update(kwds)

	def __lshift__(self, other):
		d = copy.copy(self)
		d.__dict__.update(other.__dict__)
		return d

class OptionUser(object):

	class OptionError(AttributeError): pass

	def initOptions(self, opt=Options()):
		for k, v in opt.__dict__.items():
			if(not hasattr(self.__class__, k)):
				raise self.OptionError, 'invalid option ' + k
			setattr(self, k, v)

	def reconfigureOptions(self, opt=Options(), **kwds):
		self.initOptions( opt)
		self.onReconfigureOptions(self)
		
	def onReconfigureOptions(self):
		pass

def listIt(list):
	i = 0
	while( i < len(list)):
		yield tuple(list[i:i+2])
		i += 2;		


def trivialFun(line, *args, **kwds):
	return 1


def trivialReject(line, *args, **kwds):
	return 0

class RibMod(OptionUser):	
	"""Class for modifying rib files"""
	shadowMap=0
	shadowRt=0
	updateLightParamsForShadow=0
	iMult=5000
	extrGeom=0
	remGeom=''
	inp=None
	outp=None
	funtable={}
	defShaderDir = "./shaders"
	defShaderOption = 'Option "searchpath" "shader" [".:' + defShaderDir + ':&"]'
	defTextureDir = "./textures"
	defTextureOption = 'Option "searchpath" "texture" [".:' + defTextureDir + ':&"]'
	optionVars={}
	shaderArgs=[]
	
	class FtableEntry(object):
		def __init__(self,enclosingClassInst, fname='', filterFun=None):
			self.fname= getattr(enclosingClassInst, 'process' + fname, None)
			self.filterFun = filterFun
			if(self.filterFun == None):
				self.filterFun = trivialFun
		
	def __init__(self, inp, outp, opt, **kwds):
		self.inp=inp
		self.outp=outp
		self.initOptions(opt)
		self.funtable = {
			'LightSource':self.FtableEntry(self, 'LightSource'), 
			'Display':self.FtableEntry(self, 'Display'), 
			'Surface':self.FtableEntry(self, 'Surface'), 
			'Option':self.FtableEntry(self, 'Option'),
			'WorldBegin':self.FtableEntry(self, 'WorldBegin'),
			'version':self.FtableEntry(self, 'version'),
			'ObjectBegin':self.FtableEntry(self, 'ObjectBegin'),
			'ObjectEnd':self.FtableEntry(self, 'ObjectEnd'),
			'FrameBegin':self.FtableEntry(self, 'FrameBegin'),
			'Attribute':self.FtableEntry(self, 'Attribute')
				}
		self.hasFrameBeginReached=0
		for key in self.funtable:
			assert(self.funtable[key].fname == getattr(self, 'process' + key, None))
			assert(self.funtable[key] != None)
		self.funtable['__default__'] = self.FtableEntry(self, 'DefaultPass')
		if(not self.shadowRt):
			self.funtable['Attribute'].filterFun = trivialReject
		


	def initialize(self):
		if(not self.inp):
			self.inp=sys.stdin
		else:
			self.inp=open(self.inp)
		if(not self.outp):
			self.outp=sys.stdout
		else:
			self.outp=open(self.outp, 'w')
	
		try :
			self.inp.xreadlines()
			self.inp.seek(0,0)
			self.outp.write('')
			self.outp.seek(0,0)
		except :
			raise 'class init arg, not an open file'

	def cleanup(self):		
		self.inp.close()
		self.outp.close()

	def __del__(self):
		self.cleanup()

	def do(self, *args, **kwds):
		self.initialize()
		for line in self.inp :
			outline = self.processLine(line, *args, **kwds)
			self.outp.write(outline)
		self.cleanup()

	def printUsage():
		print """
Usage :ribmodify.py <inputRibFile> <ouputRibFile> -sh -ulpfs -li <float>
	-sh, whether enable raytraced shadows
	-ulpfs, whether when rendering normal,
			to update the light parameters to include shadow map references
	-li <float>, a multiplier which is used to multiply point light intensity
	-egeom, whether to extract the geometry to the output file
	-rgeom <filename>, whether to remove the geometry and include <filename>
		as ReadArchive
"""
	
	printUsage=kgUtils.staticmethod(printUsage)		
			
		
	def parseArgs(argList):
		resList =[]
		ifname =''
		ofname=''
		options = Options()
		it = iter(argList)
		while(1):
			try:
				li = it.next()

				lindex = argList.index(li)
				if(lindex == 0):
					resList.append(li)
				elif(lindex == 1):
					resList.append(li)
				elif(li == '-sh'):
					options = options << Options(shadowRt=1)
				elif(li == '-ulpfs'):
					options = options << Options(updateLightParamsForShadow=1)
				elif(li == '-li'):
					assert(lindex < len(argList)-1)
					li = it.next()
					options =options << Options(iMult=string.atof(li))
				elif(li == '-egeom'):
					options = options << Options(extrGeom=1)
				elif(li == '-rgeom'):
					assert(lindex < len(argList)-1)
					li = it.next()
					options = options << Options(remGeom=li)
				else:
					argList=[]
					raise StopIteration
			except StopIteration:
				break
		if(argList):
			resList.append(options)
			return RibMod(*resList)
		else:
			RibMod.printUsage()
			return None
		
	parseArgs=kgUtils.staticmethod(parseArgs)
	
	def shadowLines(self):
		txt="""
PixelSamples 1 1
PixelFilter "box" 1 1
Hider "hidden" "jitter" [0]
ShadingRate 4
"""
		return txt



		
	def makeDict(self, line) :
		myDict = {}
		t = parseLine(line)
		for pair in listIt(t):
			if (len(pair) > 1 ):
				myDict[pair[0]] = pair[1]
			else:
				myDict[pair] = None
		return myDict

	
	def processDefaultPass(self, line, *args, **kwds):
		return line

	def processDefaultThrow(self, line, *args, **kwds):
		return ''
	
	def processLine(self, line, *args, **kwds) :
		
		retLine = line
		fname = self.funtable['__default__'].fname
		defRetLine1 = fname(line, self.funtable['__default__'].filterFun, *args, **kwds)		
		headExp = re.compile(r'^\s*(?P<prefix>\w+)\b(?P<rest>.*)$')
		p = headExp.match(line)
		if(p != None):
			prefix = p.group('prefix')
			if(self.funtable.has_key(prefix)):
				remLine = p.group('rest')
				fname = self.funtable[prefix].fname
				#print args
				retLine = fname(remLine.strip(), self.funtable[prefix].filterFun, *args, **kwds)				
		
		fname = self.funtable['__default__'].fname
		if(self.extrGeom):
			defRetLine2 = fname(line, self.funtable['__default__'].filterFun, *args, **kwds)	
			if(not defRetLine1 and not defRetLine2):				
				retLine = ''
		elif(self.remGeom):
			defRetLine2 = fname(line, self.funtable['__default__'].filterFun, args, kwds)	
			if(not defRetLine1 or not defRetLine2):				
				retLine = ''
		return retLine

	def processObjectBegin(self, line, filterFun, *args, **kwds):
		if(not filterFun(line, args, kwds)):
			return 'ObjectBegin' + line + '\n'			
		if(self.extrGeom):
			self.funtable['__default__'].fname = getattr(self, 'processDefaultPass', None)
			return 'ObjectBegin ' + line  + '\n'
		elif(self.remGeom):
			self.funtable['__default__'].fname = getattr(self, 'processDefaultThrow', None)
			return ''
		else:
			return 'ObjectBegin ' + line + '\n'
	
	def processFrameBegin(self, line, filterFun, *args, **kwds):
		
		if(not filterFun(line, args, kwds)):
			return 'FrameBegin ' + line + '\n'
		self.funtable['ObjectBegin'].filterFun=trivialReject
		self.funtable['ObjectEnd'].filterFun=trivialReject
		if(self.remGeom):
			return 'ReadArchive ' + '"' + self.remGeom + '"'  + '\n' + 'FrameBegin ' + line + '\n' 	
		return 'FrameBegin ' + line  + '\n'

	def processAttribute(self, line, filterFun, *args, **kwds):
		
		if(not filterFun(line, args, kwds)):
			return 'Attribute ' + line + '\n'
		d = re.match(r'^"identifier"(?P<rest>.*$)', line)
		prefix = ''
		if(d != None):
			assert(d!=None)
			remLine =  d.group('rest')
			assert(remLine != None)
			remLine = remLine.lstrip()
			d2 = self.makeDict(remLine)
			if(re.search(r'groundShape', d2.get('"name"', None))):
				prefix = 'Declare "casts_shadows" "string"\n'\
					 + 'Attribute "render" "casts_shadows" "none"\n'
			print d2
		return prefix + 'Attribute ' + line  + '\n'


	def processObjectEnd(self, line, filterFun, *args, **kwds):
		if(not filterFun(line, args, kwds)):
			return 'ObjectEnd' + line + '\n'
		if(self.extrGeom):
			self.funtable['__default__'].fname = getattr(self, 'processDefaultThrow', None)
			return 'ObjectEnd ' + line  + '\n'
		elif(self.remGeom):
			self.funtable['__default__'].fname = getattr(self, 'processDefaultPass', None)
			return ''
		else:
			return 'ObjectEnd ' + line + '\n'
		
	
		
	def processversion(self, line, filterFun, *args, **kwds):
		if(not filterFun(line, args, kwds)):
			return 'version' + line + '\n'
		if(self.extrGeom):
			self.funtable['__default__'].fname = getattr(self, 'processDefaultThrow', None)
		return 'version ' + line + '\n'
	
	def processLightSource(self, line, filterFun, *args, **kwds) :
		global rexp1		
		if(not filterFun(line, args, kwds)):
			return 'LightSource' + line + '\n'
		d = self.makeDict(line)
		if( d.has_key('"pointlight"') and d.has_key('"intensity"') ) :
			val = d['"intensity"']
			m = rexp_1.match(val)
			if(m):			
				f = float(m.group('num'))
				f *= self.iMult
				d['"intensity"'] = '[' + str(f) + ']'
			else:
				raise  myException
		list1 = ['LightSource']
		prefix = ''
	
		if(self.updateLightParamsForShadow and (d.has_key('"pointlight"') or d.has_key('"distantlight"'))):
			d.update({'"bias"': '[' + str(0.45) + ']', \
				'"samples"': '[' + str(16) + ']',
				'"blur"': '[' + str(0.01) + ']',
				'"shadowname"': '""'})
			prefix="""
Declare "bias" "float"
Declare "samples" "float"
Declare "shadowname" "string"
Declare "blur" "float"
				"""


		if(self.shadowRt and (d.has_key('"pointlight"') or d.has_key('"distantlight"'))):
			prefix='Attribute "light" "shadows" "on"\n'\
				+ prefix
			
		for key in d:
			list1.append(key)
			list1.append(d[key])
		retLine = ' '.join(list1) + '\n'

		if(prefix):

			retLine = prefix + '\n' + retLine

		return retLine

	def processWorldBegin(self, line, filterFun, *args, **kwds) :		
		retLine = 'WorldBegin ' + line + '\n'
		if(not filterFun(line, args, kwds)):
			return 'Worldbegin' + line + '\n'
		if(self.optionVars.get('"searchpath"', {}).get('"shader"', None) == None \
			and self.defShaderOption != ''):
			retLine = self.defShaderOption + '\n' + retLine
		if(self.optionVars.get('"searchpath"', {}).get('"texture"', None) == None \
			and self.defTextureOption != ''):
			retLine = self.defTextureOption + '\n' + retLine	
		if(self.shadowMap):
			retLine = self.shadowLines() + retLine
		if(self.shadowRt):
			retLine = 'Declare "shadows" "string"' + '\n' \
				  + 'Declare "casts_shadows" "string"' + '\n' \
				  + retLine
		print retLine
		return retLine

	def processDisplay(self, line, filterFun, *args, **kwds) :
		retLine = line	
		if(not filterFun(line, args, kwds)):
			return 'Display' + retLine + '\n'
		if(self.shadowMap):
			return 'Display ' +  '"shad_'\
			 + os.path.splitext(os.path.basename(self.inp.name))[0] +\
			 '.z"' + '  ' + '"zfile" "z"' + '\n'
		d = re.match(r'^"(?P<fname>\w+(?:[.]\w+)?)"', line)
		assert(d!=None)
		imagename = d.group('fname')
		assert(imagename != '')
		f = re.search(r'(?P<ftitle>\w+)(?P<rest>[.]\w+)?', os.path.basename(self.inp.name))
		assert(f != None)
		g = re.search(r'(?P<ftitle>\w+)(?P<rest>[.]\w+)?', os.path.basename(self.outp.name))
		assert(g != None)		
		e=re.compile(r'' + os.path.splitext(os.path.basename(self.inp.name))[0] )
		retLine = e.sub(os.path.splitext(os.path.basename(self.outp.name))[0], line)
		retLine = 'Display ' + retLine+ '\n'	
		return retLine


	def processOption(self, line, filterFun, *args, **kwds) :
		retLine = line
		if(not filterFun(line, args, kwds)):
			return 'Option' + retLine + '\n'		
		d = re.match(r'^"searchpath"(?P<rest>.*$)', line)
		if(d != None):
			assert(d!=None)
			remLine =  d.group('rest')
			assert(remLine != None)
			remLine = remLine.lstrip()	
			u = self.makeDict(remLine)
			spe = self.optionVars.setdefault('"searchpath"', {}).update(u)	
		d = re.match(r'^"render"(?P<rest>.*$)', line)
		if(d != None):
			assert(d!=None)
			remLine =  d.group('rest')
			assert(remLine != None)
			remLine = remLine.lstrip()	
			u = self.makeDict(remLine)
			spe = self.optionVars.setdefault('"render"', {}).update(u)	


		retLine = 'Option ' + retLine	+ '\n'	
		return retLine



	def processSurface(self, line, filterFun, *args, **kwds) :
		retLine = line
		if(not filterFun(line, args, kwds)):
			return 'Surface' + retLine + '\n'

		d = re.match(r'^"(?P<shaderName>\w+)"(?P<rest>.*)$', line)
		assert(d!=None)
		shaderName = d.group('shaderName')
		assert(shaderName != '')
		rest = d.group('rest')
		assert(rest != None)
		rest = rest.strip()

		d = self.makeDict(rest)
		decl = ''
		for key in d:
			if(d[key] == ''):
				continue
			try :
				f = string.atof(d[key])
				decl += '\nDeclare ' + key + ' "float"'
			except ValueError:
				decl += '\nDeclare ' + key + ' "string"'
		retLine = decl.lstrip() + '\n' + 'Surface ' + retLine	+ '\n'	
		return retLine

	



if __name__ == "__main__" :
	nargs = len(sys.argv)

	print 'current wd is ' , os.getcwd()
	ribmod = RibMod.parseArgs(sys.argv[1:])
	if(ribmod): ribmod.do()




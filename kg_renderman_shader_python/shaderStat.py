from __future__ import generators
import os, sys, string, exceptions
import tempfile, ribmodify, kgUtils, math, re, fpformat, operator

#This is a utility to do iterative rendering
#on a rib file for a  particular shader.
# Suppose myRibFile.rib has a shader caled
#  banana, with the following parameters
# (Ka, float), (Kd=float), (ks, float), (spotDensity, float),
# (wrinkleDensity, float)



#For using this utility,
#	1)you have to first specify the iterative mechanism
#	of all the potential candidate shader parameters.
#	In the above example,
# 	if you want to iterate on wrinkleDensity and spotDensity
#	change the shaderParamDict to
#	shaderParamDict={'wrinkleDensity':kgUtils.curry(getNextVal, minVal1, maxVal1, fun11, fun21)(),
#			'spotDensity':kgUtils.curry(getNextVal, minVal2, maxVal2, fun21, fun22)()
#			}
#	minVal1, maxVal1 are the minimum and maximum values you
#	want to iterate for the shader parameter.
#	fun11, fun12 are two modifying functions
#	fun12 should be inverse of fun1
#	If you just need a linear stepping iterataion,
#	fun12 and fun12 can be trivial lambda functions
#	In some cases, you need to provide logarithmic and exponential
#	functions
#
#
#	2)After this,
#	if you want to iterate through just the wrinkleDensity,
#	you have to call this
#	utility as
#	shaderStat.py myRibFile.rib banana wrinkleDensity
#
#This will render the ribfile, repeatedlyusing bmrt
#and produce a  bunch of temporary ribfiles and rendering output tiff files.
#Each file will be named by the apropriate shading parameter, abbreviation



#A utility function,
#which makes a generator function for the shading parameter.
#Each call of next, by this function will,
#get the next value of the shader parameter
#I have used 'generator mechanism' which
#is available in python2.2.1

def getNextVal(min, max, fun, invFun,  numSteps):	
	assert(callable(fun) and callable(invFun))
	assert(repr(numSteps).isdigit())
	assert(numSteps > 0)
	assert(fun(min) <= fun(max))
	stepWidth = (fun(max) - fun(min))/numSteps
	while(1):
		lastVal = fun(max)
		while(lastVal > fun(min)):
			yield invFun(lastVal)
			lastVal = lastVal - stepWidth



shaderParamDict={'opacity' : kgUtils.curry(getNextVal, 0.1, 1.0, lambda x: x, lambda x: x, 5)(), 
   'noiseFreq' : kgUtils.curry(getNextVal, 0.001, 100, math.log10, lambda x: math.pow(10, x), 10)(),
   'namp' : kgUtils.curry(getNextVal, 0, 1, lambda x: x, lambda x: x, 1)()
   }



#A utility function to construct
#afilename from the current permutation of shader parameters


def constructFileName(fname, shaderName, shaderParams, shaderArgs):
	assert(kgUtils.isAString(shaderName) and kgUtils.isAString(fname))
	assert(operator.isSequenceType(shaderParams) and operator.isSequenceType(shaderArgs))
	fname = os.path.abspath(fname)
	fname1, ext = os.path.splitext(fname)
	resFileName=fname1
	if(shaderParams != None or shaderArgs != None):	
		print shaderParams
		print shaderArgs
		resFileName +=  '_' + '_'.join([ x[:2]\
					  + '_' + re.sub(r'[.]', '_', y) \
					  for x, y in map(None, shaderParams, shaderArgs)])
	resFileName += ext
	print resFileName
	return resFileName

def constructFileNameForExtractingGeom(fname):
	#fname = os.path.abspath(fname)
	fname1, ext = os.path.splitext(fname)
	return fname1 + '_geom' + ext

#A filter function,
#which filters out any other shader
#which is not the one for which this utility
#is used for

def filterFunSurface(line, *args, **kwds):
	retLine = line
	d = re.match(r'^"(?P<shaderName>\w+)"(?P<rest>.*)$', line)
	assert(d!=None)
	shaderName = d.group('shaderName')
	assert(shaderName != '')
	if(shaderName == 'morange'):
		return 1
	return 0

#An enhancement of the method
#RibMod.processSurface
#Which is tailored for this particular utility

def myProcessSurface(defMethod, self, line, filterFun, *args, **kwds):
	retLine = line
	if(not filterFun(line, None)):
		return 'Surface' + line + '\n'	
	d = re.match(r'^"(?P<shaderName>\w+)"(?P<rest>.*)$', line)
	assert(d!=None)
	shaderName = d.group('shaderName')
	assert(shaderName != '')
	rest = d.group('rest')
	assert(rest != None)
	rest = rest.strip()
	
	assert(shaderName == 'morange')
	assert(kgUtils.isAString(args[0]))
	
	
	d = self.makeDict(args[0])
	print d
	decl = ''
	for key in d:
		if(d[key] == ''):
			continue
		try :
			c = re.match(r'"(?P<arg>.+)"', d[key])
			if(c != None):
				e = c.group('arg')
			else :
				e = d[key]							
			print 'e:',c, e
			f = string.atof(e)
			decl += '\nDeclare ' + key + ' "float"'
		except ValueError:
			decl += '\nDeclare ' + key + ' "string"'
	retLine = decl.lstrip() + '\n' + 'Surface ' + '"' + shaderName +'" ' + args[0]	+ '\n'
	print retLine
	return retLine


#A utility function which gets the
#available shader parameters of a
#given shader using the
#slctell utility


def getShaderParams(shName):

	retVal = {}
	oldDir = os.getcwd()
	tfname = tempfile.mktemp('.dat')
	tfname = os.path.abspath(tfname)
	tinput = open(tfname, 'w+')
	tinput.close()
	try :
		os.chdir(ribmodify.RibMod.defShaderDir)	
		shName1, shName2 = os.path.splitext(shName)
		shNameWithExt = shName
		if(shName2 != None):
			shNameWithExt = shName + '.slc'
		ret = os.system('slctell ' + shNameWithExt + ' >' +  tfname)
		if(ret != 0):
			return
		tinput = open(tfname)
		listOfLines = list(tinput)
		listOfLines = listOfLines[1:]	
		if (not listOfLines):
			return
		it = iter(listOfLines)
		while(1):			
			try:				
				l1 = it.next()
				l2 = it.next()			
				l11 = l1.split('"')
				l11 = filter(lambda x: x.strip() != '', l11)
				l21 = ribmodify.parseLine(l2)
				if(len(l11[1].split()) > 1):
					paramType = l11[1].split()[1]
				else:
					paramType = l11[1]
				val = l21[2]
				if(paramType == 'color'):
					val = l21[3]			
				retVal[l11[0]] = (paramType, ribmodify.ribTypeCast(paramType, val))				
			except StopIteration:
				break
		tinput.close()

	finally:
		os.chdir(oldDir)
		tinput.close()
		os.remove(tfname)
		return retVal
	


	
			

if __name__ == "__main__" :
	nargs = len(sys.argv)
	assert(nargs > 3)
	assert(len(os.path.splitext(sys.argv[1])) == 2\
	 and  string.lower(os.path.splitext(sys.argv[1])[1]) == '.rib')
	
	
	#enhance the method 'processSurface' of class RibMod to
	#myProcessSurface which is tailored to do
	#the stuff for this particular utility
	
	shaderName = sys.argv[2]
	

	geomFileName = constructFileNameForExtractingGeom(sys.argv[1])
	argList=[sys.argv[1], geomFileName, '-egeom']
	ribmod= ribmodify.RibMod.parseArgs(argList)
	assert(ribmod != None)
	ribmod.funtable.setdefault('Surface', ribmod.FtableEntry(ribmod, 'Surface')).filterFun=filterFunSurface					
	ribmod.do()
	kgUtils.enhance_method(ribmodify.RibMod, 'processSurface', myProcessSurface)	
	#just get all possible
	#shader parameters fpr the shader in question
	#This is just for validity check
	
	shaderParams = getShaderParams('morange')
	global shaderparamDict
	numSteps = 10
	numIter = int(pow(numSteps, nargs-3))
	f1 = kgUtils.curry(getNextVal, 0, 1, lambda x: x, lambda x: x)
	#for each argument shader parameters
	for i in range(nargs-3):
		shaderParam = sys.argv[i+3]
		#check it is present in shaderParamDict
		assert(shaderParamDict.has_key(shaderParam))
		#assert(shaderParams.has_key(shaderParam))
	#iterate
	shaderArgs = [ 0 for i in range(nargs-3)]
	for i in xrange(numIter):
		p = 1
		#get a list of updated shader parameters
		for j in range(nargs-3):
			#if this shader param need be changed do it
			if(not i % p):
				shaderParam = sys.argv[j+3]
				shaderArgs[j] = round(shaderParamDict[shaderParam].next(), 2)
				assert(shaderParams.has_key(shaderParam))
				assert(len(shaderParams[shaderParam])>1)
				paramType = shaderParams[shaderParam][0]
				shaderArgs[j] = ribmodify.ribTypeCast(paramType, shaderArgs[j])
			p = p * numSteps
		#constriuct a new filename for this rendering
		tfname = constructFileName(sys.argv[1], 'morange', sys.argv[3:],  [str(fpformat.fix(i, 2)) for i in shaderArgs])
		#construct a shader argument string to be
		#updated in the new rib file
		shaderArgStr= ''.join([ '"' + sys.argv[i+3] + '" "' + str(fpformat.fix(shaderArgs[i], 2))+ '" ' for i in range(len(shaderArgs))])	
		print shaderArgStr
		#open the input rib file
		#input = open(sys.argv[1])
		#open the temporary file with the newly constructed filename
		#toutput = open(tfname, 'w+')
		#make an instacne of RibMod
		argList = [sys.argv[1], tfname, '-rgeom', os.path.basename(geomFileName)]
		ribmod = ribmodify.RibMod.parseArgs(argList)
		#set the filter function for
		#surface to avoid all surface calls
		if(ribmod != None):		
			ribmod.funtable.setdefault('Surface', ribmod.FtableEntry(ribmod, 'Surface')).filterFun=filterFunSurface			
			#do the processing
			ribmod.do(shaderArgStr)
			#call the bmrt call to render the new ribfile
			
			ret = os.system('rendrib ' + '"' + tfname + '"')
			print ret, tfname




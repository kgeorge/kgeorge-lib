import new




# This is a utility function which is
# used to create another function
# from an argument function
# Let fun1(arg1, arg2, arg3, kw1=val1,kw2=val2,kw3=val3)
# be a standard function
# fun2=curry(fun1, arg1, arg2, kw1:val2, kw3=val3)
# will be  a new function, created from fun1
# and can be called fun2(arg2, kw2=val2)
def curry(fun, *args, **kwds):
    def callit(*moreArgs, **moreKwds):
        kw = kwds.copy()
        kw.update(moreKwds)
        return fun(*(args + moreArgs), **kw)

    return callit


#This is used to make a static method in a class
#For eg:
#class A:
#   def method(*args, **kwds):
#           pass
#   method=staticmethod(method)
#
#
# Now you can call the method as
# A.method(args, kwds)
#ie you dont need an instance


class staticmethod:
    def __init__ (self, callable):
            self.__call__ = callable

#This is used to enhance dynamically an already existing method
#of a class in a third party module, without going through
#the usual overhead of deriving another class.
#This way you can get the enhanced method
#without changing the third party module.
#
#Eg:
#def fun2(*args, **kwds):
#   print 'hi'
#
#class A:
#   def fun1(*args, **kwds):
#       pass
#enhance_method(A, 'fun1', fun2)
#a= A()
#a.fun1() -will print 'hi'

            
def enhance_method(klass, method_name, replacement):
    method = getattr(klass, method_name, None)
    assert(method != None)
    def enhanced(*args, **kwds):
        return replacement(method, *args, **kwds)
    setattr(klass, method_name, new.instancemethod(enhanced, None, klass))




    
def isAString(s):
	try: s + ''
	except: return 0
	else: return 1

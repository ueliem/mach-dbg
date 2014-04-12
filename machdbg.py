import os, sys
from subprocess import *
import time
from ctypes import *

if __name__ == '__main__':
	libc = CDLL('libc.dylib')
	if not libc:
		print "Not working"
		raise Exception("Not working")

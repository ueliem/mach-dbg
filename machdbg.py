import os, sys, argparse
from subprocess import *
import time
from ctypes import *

libc = None

#commands = ["run", "quit"]

def parse_command(tokens):
	if(tokens[0] == "quit"):
		sys.exit(0)

def main():
	libc = CDLL('libc.dylib')
	if not libc:
		print "Failed to load C Library"
		raise Exception("Failed to load C Library")

	parser = argparse.ArgumentParser()
	parser.add_argument('executable', nargs='?', help='executable help')
	args = parser.parse_args()
	executable = args.executable
	if executable == None:
		print "No executable"
	else:
		print "Gave executable " + executable

	while True:
		command = raw_input("(mdbg) ")
		tokens = command.split(' ')
		print tokens[0]
		if tokens[0] in ["run", "quit"]:
			parse_command(tokens)

if __name__ == '__main__': main()
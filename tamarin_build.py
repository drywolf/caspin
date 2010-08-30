#!/usr/bin/env python

import os;
import sys;
import colors;
import shutil;

if os.name != "posix":
	colors.printc("red", "ERROR: only usable on unix systems")
	sys.exit(0)

# the initial working directory
cwd = os.getcwd()

os.chdir(cwd + "/tamarin/bin")



build_debug_debugger =		raw_input("build the DEBUG_DEBUGGER configuration? [y/n]\n")
build_debug =				raw_input("build the DEBUG configuration? [y/n]\n")
build_release_debugger =	raw_input("build the RELEASE_DEBUGGER configuration? [y/n]\n")
build_release =				raw_input("build the RELEASE configuration? [y/n]\n")



# debug debugger build
if build_debug_debugger == "y":
	colors.printc("yellow", "configuring tamarin for DEBUG_DEBUGGER build...")

	os.system("../configure.py --enable-debug --enable-debugger")

	colors.printc("yellow", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

	# rename the output files
	shutil.move("libavmplus.a",	"libavmplus_debugger_d.a");
	shutil.move("libMMgc.a",	"libMMgc_debugger_d.a");



# debug build
if build_debug == "y":
	colors.printc("yellow", "configuring tamarin for DEBUG build...")

	os.system("../configure.py --enable-debug")

	colors.printc("yellow", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

	# rename the output files
	shutil.move("libavmplus.a",	"libavmplus_d.a");
	shutil.move("libMMgc.a",	"libMMgc_d.a");
	shutil.move("libzlib.a",	"libzlib_d.a");



# release debugger build
if build_release_debugger == "y":
	colors.printc("yellow", "configuring tamarin for RELEASE_DEBUGGER build...")

	os.system("../configure.py --enable-debugger")

	colors.printc("yellow", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

	# rename the output files
	shutil.move("libavmplus.a",	"libavmplus_debugger.a");
	shutil.move("libMMgc.a",	"libMMgc_debugger.a");



# release build
if build_release == "y":
	colors.printc("yellow", "configuring tamarin for RELEASE build...")

	os.system("../configure.py")

	colors.printc("yellow", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

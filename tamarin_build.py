#!/usr/bin/env python

import os;
import sys;
import colors;
import shutil;

if os.name != "posix":
	colors.println("r", "ERROR: only usable on unix systems")
	sys.exit(0)

# the initial working directory
cwd = os.getcwd()

if not os.path.exists(cwd + "/tamarin/bin"):
	os.mkdir(cwd + "/tamarin/bin")

os.chdir(cwd + "/tamarin/bin")

def build_config(config):
	colors.printf("d", "build the ")
	colors.printf("r", config)
	colors.println("d", " configuration? [y/n]")
	return raw_input("")



build_debug_debugger =		build_config("DEBUG_DEBUGGER")
build_debug =				build_config("DEBUG")
build_release_debugger =	build_config("RELEASE_DEBUGGER")
build_release =				build_config("RELEASE")



# debug debugger build
if build_debug_debugger == "y":
	colors.println("y", "configuring tamarin for DEBUG_DEBUGGER build...")

	os.system("../configure.py --enable-debug --enable-debugger")

	colors.println("y", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

	# rename the output files
	shutil.move("libavmplus.a",	"libavmplus_debugger_d.a");
	shutil.move("libMMgc.a",	"libMMgc_debugger_d.a");



# debug build
if build_debug == "y":
	colors.println("y", "configuring tamarin for DEBUG build...")

	os.system("../configure.py --enable-debug")

	colors.println("y", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

	# rename the output files
	shutil.move("libavmplus.a",	"libavmplus_d.a");
	shutil.move("libMMgc.a",	"libMMgc_d.a");
	shutil.move("libzlib.a",	"libzlib_d.a");



# release debugger build
if build_release_debugger == "y":
	colors.println("y", "configuring tamarin for RELEASE_DEBUGGER build...")

	os.system("../configure.py --enable-debugger")

	colors.println("y", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

	# rename the output files
	shutil.move("libavmplus.a",	"libavmplus_debugger.a");
	shutil.move("libMMgc.a",	"libMMgc_debugger.a");



# release build
if build_release == "y":
	colors.println("y", "configuring tamarin for RELEASE build...")

	os.system("../configure.py")

	colors.println("y", "compiling tamarin...")
	os.system("make clean")
	os.system("make")

#!/usr/bin/env python

import os;
import sys;
import colors;
import shutil;
import urllib2;

# the tamarin mercurial repository url
tamarin_repository = "http://hg.mozilla.org/tamarin-redux"

# the tamarin revision that shall be used with this version of caspin
tamarin_revision = "95ff632db74b"

# the initial working directory
cwd = os.getcwd()

# clear the tamarin directory
if os.path.exists(cwd + "/tamarin"):
	colors.printc("yellow", "clearing tamarin directory...")
	shutil.rmtree(cwd + "/tamarin")

# the mercurial command
cmd = "hg clone -r " + tamarin_revision + " " + tamarin_repository + " " + cwd + "/tamarin"

colors.printc("yellow", "checking out tamarin...\n(this may take serveral minutes depending on your internet connection)")

# invoke the mercurial command to check out tamarin
os.system(cmd);

# on unix we can start the build process automatically
if os.name == "posix":
	colors.printc("yellow", "configuring tamarin...")
	os.mkdir(cwd + "/tamarin/bin")
	os.chdir(cwd + "/tamarin/bin")
	os.system("../configure.py")
	
	colors.printc("yellow", "compiling tamarin...")
	os.system("make")

# apply the changes to tamarin that are necessarry on win32
elif os.name == "nt":
	colors.printc("yellow", "patching tamarin for use on win32...")
	os.chdir(cwd + "/tamarin/")
	os.system("hg patch --no-commit ../tamarin_changes.diff")

# download asc.jar
colors.printc("yellow", "downloading \"asc.jar\"...")
jar_url = urllib2.urlopen("ftp://ftp.mozilla.org/pub/js/tamarin/builds/asc/latest/asc.jar")
jar_file = open(cwd + "/tamarin/utils/asc.jar", "wb")
jar_file.write(jar_url.read())
jar_file.close()

# display some final information
if os.name == "posix":
	colors.printc("yellow", "To make use of the capin tools (cspcompile, cspgenerate, etc.) you need to set up the 'CASPIN_HOME' environment variable inside your '~home/.bash_profile' config file.")
	colors.printc("yellow", "Copy the following text to the end of the file: 'export CASPIN_HOME=" + cwd + "'")

elif os.name == "nt":
	colors.printc("yellow", "To make use of the capin tools (cspcompile, cspgenerate, etc.) you need to set up the 'CASPIN_HOME' environment variable.")
	colors.printc("yellow", "Execute the following command at the command prompt: 'setx CASPIN_HOME " + cwd + "'")

colors.printc("green", "finished setup procedure...")

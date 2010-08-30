#!/usr/bin/env python

import os;
import sys;

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# a list of all native classes
classes = [
caspin_home + "/tamarin/shell/Domain.as", 
caspin_home + "/tamarin/shell/ByteArray.as"
]

# base command for compiling the native class package
cmd = "python " + caspin_home + "/tools/cspcompile.py flash_utils ";

# add the classes to the build command
for str in classes:
	cmd += str + " ";

# compile the native package
os.system(cmd);

# generate the glue code for the native package
os.system("python " + caspin_home + "/tools/cspgenerate.py flash_utils");

# compile our sample minimal.as
os.system("python " + caspin_home + "/tools/cspcompile.py minimal minimal.as");

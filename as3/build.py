#!/usr/bin/env python

import os
import sys

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# a list of all native classes
classes = [
"caspin_base.as"
]

# base command for compiling the native class package
cmd = caspin_home + "/tools/cspcompile.py caspin_base "

# add the classes to the build command
for str in classes:
	cmd += str + " "

# compile the native package
os.system(cmd)

# generate the glue code for the native package
os.system(caspin_home + "/tools/cspgenerate.py caspin_base")

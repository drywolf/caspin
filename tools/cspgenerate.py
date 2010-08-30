#!/usr/bin/env python

import os;
import sys;
import shutil;

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# base command for invoking nativegen.py (automatically includes the tamarin "builtin.abc")
cmd = "python " + caspin_home + "/tamarin/utils/nativegen.py " + caspin_home + "/tamarin/core/builtin.abc ";

# concatenate all arguments except for the first one
for i in range(2, len(sys.argv)):
	cmd += sys.argv[i] + " ";

# use the first argument as output filename
cmd += sys.argv[1] + ".abc";

# generate the C++ glue code from .abc files
os.system(cmd);

# rename and move the cpp/h files
shutil.move(sys.argv[1] + ".cpp2", "../src/" + sys.argv[1] + ".cpp");
shutil.move(sys.argv[1] + ".h2", "../include/" + sys.argv[1] + ".h");

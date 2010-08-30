#!/usr/bin/env python

import os;
import sys;

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# the base command for the ActionScript compiler
cmd = "java -ea -DAS3 -DAVMPLUS -classpath " + caspin_home + "/tamarin/utils/asc.jar macromedia.asc.embedding.ScriptCompiler -out ";

# append the output filename
cmd += sys.argv[1];

# import the default tamarin builtin code
cmd += " -builtin -import " + caspin_home + "/tamarin/core/builtin.abc ";

# append all requested .as files
for i in range(2, len(sys.argv)):
	cmd += sys.argv[i] + " ";

# a small hack for asc.jar to navigate back to the correct working directory
# otherwise the output file wouldn't be placed correctly
dummy_file = open("dummy_file.as", "w")
dummy_file.close()

# invoke the ActionScript compiler
os.system(cmd + " dummy_file.as");

# remove the dummy file
os.remove("dummy_file.as");

# remove the deprecated cpp/h files
os.remove(sys.argv[1] + ".h");
os.remove(sys.argv[1] + ".cpp");

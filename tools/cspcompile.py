#!/usr/bin/env python

import os
import sys
import time
import files

# if another instance of cspcompile is already running at this working directory
# suspend this instance until the other has finished
while os.path.exists("dummy_file.as"):
	time.sleep(1)

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# the output filename (without file extension)
outfile = sys.argv[1]
outfile_cpp = outfile + ".cpp"
outfile_h = outfile + ".h"

# get the modification timestamp of the output file
if os.path.exists(outfile + ".abc"):
	outfile_mod_date = os.path.getmtime(outfile + ".abc")
else:
	outfile_mod_date = 0

# the base command for the ActionScript compiler
cmd = "java -ea -DAS3 -DAVMPLUS -classpath " + caspin_home + "/tamarin/utils/asc.jar macromedia.asc.embedding.ScriptCompiler -out "

# append the output filename
cmd += outfile

# import the default tamarin builtin code
cmd += " -builtin -import " + caspin_home + "/tamarin/core/builtin.abc "

build_required = False

for i in range(2, len(sys.argv)):
	# append all requested .as files
	cmd += sys.argv[i] + " "
	
	# check if at least one file has been modified since the previous build
	file_list = files.search(sys.argv[i])
	
	for file in file_list:
		mod_date = os.path.getmtime(file)
		build_required = build_required or mod_date >= outfile_mod_date

# only continue if we need to
if build_required:
	print "build is required!"
else:
	print "build is not required, skipping..."
	exit()

# a small hack for asc.jar to navigate back to the correct working directory
# otherwise the output file wouldn't be placed correctly
dummy_file = open("dummy_file.as", "w")
dummy_file.close()

# invoke the ActionScript compiler
os.system(cmd + " dummy_file.as")

# remove the dummy file
if os.path.exists("dummy_file.as"):
	os.remove("dummy_file.as")

# remove the deprecated cpp/h files
if os.path.exists(outfile_h):
	os.remove(outfile_h)
	
if os.path.exists(outfile_cpp):
	os.remove(outfile_cpp)

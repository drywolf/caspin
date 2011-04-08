#!/usr/bin/env python

import colors
import os
import shutil
import sys
import fileinput

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# the input filename (without file extension)
infile = sys.argv[1]
infile_abc = infile + ".abc"
outfile_cpp = "../src/" + infile + ".cpp"
outfile_h = "../include/" + infile + ".h"

infile_mod_date = 1

# get the modification timestamp of the input file
if os.path.exists(infile_abc):
	infile_mod_date = os.path.getmtime(infile_abc)

# base command for invoking nativegen.py (automatically includes the tamarin "builtin.abc")
cmd = "python " + caspin_home + "/tamarin/utils/nativegen.py " + caspin_home + "/tamarin/core/builtin.abc "

outfile_mod_date = 0

# get the modification timestamps of the output files
if os.path.exists(outfile_cpp):
	outfile_mod_date = os.path.getmtime(outfile_cpp)
	
if os.path.exists(outfile_h):
	mod_date = os.path.getmtime(outfile_h)
	if mod_date > outfile_mod_date:
		outfile_mod_date = mod_date

gen_required = infile_mod_date >= outfile_mod_date

# only continue if we need to
if gen_required:
	colors.printf("r", infile + ":")
	colors.println("d", "\tglue generation required...")
else:
	colors.printf("g", infile + ":")
	colors.println("d", "\tup to date...")
	exit()

for i in range(2, len(sys.argv)):
	# append all imported .abc files
	cmd += sys.argv[i] + " "

# use the first argument as output filename
cmd += infile_abc

#if len(sys.argv) > 2:
#	cmd += " --native-id-namespace " + sys.argv[2] + " "
# generate the C++ glue code from .abc files
result = os.system(cmd)

if result == 0:
	# rename and move the cpp/h files
	if os.path.exists(infile + ".cpp2"):
		shutil.move(infile + ".cpp2", outfile_cpp)

	if os.path.exists(infile + ".h2"):
		shutil.move(infile + ".h2", outfile_h)
else:
	# clear the output
	if os.path.exists(outfile_cpp):
		os.remove(outfile_cpp)

	if os.path.exists(outfile_h):
		os.remove(outfile_h)

file = open(outfile_h, "r")
file2 = open(outfile_h+"2", "w")

file2.write("#ifndef __" + infile.upper() + "_GLUE_H__\n")
file2.write("#define __" + infile.upper() + "_GLUE_H__\n")
file2.write("\n")

line = file.readline()
while (line != ""):
	line = line.replace("AvmString", "avmplus::AvmString")
	line = line.replace("AvmObject", "avmplus::AvmObject")
	line = line.replace("AvmBool32", "avmplus::AvmBool32")
	line = line.replace("AvmBox", "avmplus::AvmBox")
	file2.write(line)
	line = file.readline()

file2.write("\n")
file2.write("#endif // __" + infile.upper() + "_GLUE_H__\n")

file.close()
file2.close()

shutil.move(outfile_h + "2", outfile_h)
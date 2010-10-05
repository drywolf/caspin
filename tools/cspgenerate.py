#!/usr/bin/env python

import os
import shutil
import sys

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# the input filename (without file extension)
infile = sys.argv[1]
infile_abc = infile + ".abc"
outfile_cpp = "../src/" + infile + ".cpp"
outfile_h = "../include/" + infile + ".h"

infile_mod_date = 0

# get the modification timestamp of the input file
if os.path.exists(infile_abc):
	infile_mod_date = os.path.getmtime(infile_abc)

# base command for invoking nativegen.py (automatically includes the tamarin "builtin.abc")
cmd = "python " + caspin_home + "/tamarin/utils/nativegen.py " + caspin_home + "/tamarin/core/builtin.abc "

outfile_mod_date = -1

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
	print "code generation is required!"
else:
	print "code generation is not required, skipping..."
	exit()

# use the first argument as output filename
cmd += infile_abc

# generate the C++ glue code from .abc files
os.system(cmd)

# rename and move the cpp/h files
if os.path.exists(infile + ".cpp2"):
	shutil.move(infile + ".cpp2", outfile_cpp)

if os.path.exists(infile + ".h2"):
	shutil.move(infile + ".h2", outfile_h)

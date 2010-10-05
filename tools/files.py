#!/usr/bin/env python

import os
import re
import shutil
import string

# a helper function to search files by a given wildcard pattern
def search(pattern):
	directory = os.path.dirname(pattern)
	regex_pattern = os.path.basename(pattern)
	
	if len(directory) == 0:
		directory = "./"
	
	regex_pattern = string.replace(regex_pattern, ".", "\\.")
	regex_pattern = string.replace(regex_pattern, "*", ".*")

	fileList = os.listdir(directory)
	regex = re.compile(regex_pattern)
	return [directory + "/" + f for f in fileList if regex.match(f) != None]

def copy(src, dest):
	file_list = search(src)
	for file in file_list:
		shutil.copy(file, dest)

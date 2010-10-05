#!/usr/bin/env python

import os

os.chdir("../tamarin/")
os.system("hg diff -p > ../tamarin_changes.diff.new")

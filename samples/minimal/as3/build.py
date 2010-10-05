#!/usr/bin/env python

import os

# the CASPIN_HOME environment variable
caspin_home = os.getenv("CASPIN_HOME")

# compile our sample minimal.as
os.system("python " + caspin_home + "/tools/cspcompile.py minimal minimal.as")

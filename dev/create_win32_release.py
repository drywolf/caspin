#!/usr/bin/env python

import sys
sys.path.append("../tools/")

import files
import os
import shutil
import time

if len(sys.argv) < 2:
	print "Error: no target specified"
	exit()

def create_dirs(target):
	if os.path.exists(target):
		print "clearing " + target + " dirs"
		shutil.rmtree(target)
	
	time.sleep(1)
	
	print "creating " + target + " dirs"
	os.mkdir(target)
	os.mkdir(target + "/caspin")

	# toplevel directories
	os.mkdir(target + "/caspin/include")
	os.mkdir(target + "/caspin/lib")
	os.mkdir(target + "/caspin/lib/Release")
	os.mkdir(target + "/caspin/lib/Debug")
	os.mkdir(target + "/caspin/tools")
	
	os.mkdir(target + "/caspin/tamarin")
	os.mkdir(target + "/caspin/tamarin/core")
	os.mkdir(target + "/caspin/tamarin/vmpi")
	os.mkdir(target + "/caspin/tamarin/shell")
	os.mkdir(target + "/caspin/tamarin/platform")
	os.mkdir(target + "/caspin/tamarin/platform/win32")
	os.mkdir(target + "/caspin/tamarin/mmgc")
	os.mkdir(target + "/caspin/tamarin/nanojit")
	os.mkdir(target + "/caspin/tamarin/eval")
	os.mkdir(target + "/caspin/tamarin/extensions")
	os.mkdir(target + "/caspin/tamarin/utils")

def copy_tamarin_headers(target):

	print ">> core"
	files.copy("../tamarin/core/*.h",				target + "/caspin/tamarin/core/")
	print ">> vmpi"
	files.copy("../tamarin/vmpi/*.h",				target + "/caspin/tamarin/vmpi/")
	print ">> platform"
	files.copy("../tamarin/platform/*.h",			target + "/caspin/tamarin/platform/")
	files.copy("../tamarin/platform/win32/*.h",	target + "/caspin/tamarin/platform/win32/")
	print ">> mmgc"
	files.copy("../tamarin/mmgc/*.h",				target + "/caspin/tamarin/mmgc/")
	print ">> nanojit"
	files.copy("../tamarin/nanojit/*.h",			target + "/caspin/tamarin/nanojit/")
	print ">> eval"
	files.copy("../tamarin/eval/*.h",				target + "/caspin/tamarin/eval/")
	print ">> extensions"
	files.copy("../tamarin/extensions/*.h",		target + "/caspin/tamarin/extensions/")

	shutil.copy("../tamarin/core/opcodes.tbl",			target + "/caspin/tamarin/core/")
	shutil.copy("../tamarin/core/builtin.abc",			target + "/caspin/tamarin/core/")
	shutil.copy("../tamarin/shell/avmshell-features.h",	target + "/caspin/tamarin/shell/")
	shutil.copy("../tamarin/utils/asc.jar",				target + "/caspin/tamarin/utils/")
	shutil.copy("../tamarin/utils/nativegen.py",			target + "/caspin/tamarin/utils/")

def copy_tamarin_libs_vc9():

	obj_dir = "../tamarin/platform/win32/obj_9/"
	lib_release = "vc9/caspin/lib/Release/"
	lib_debug = "vc9/caspin/lib/Debug/"

	#vc9_release
	# avmplus
	shutil.copy(obj_dir + "avmplus/Release/avmplus.lib",			lib_release)
	shutil.copy(obj_dir + "avmplus/Debug/avmplus.lib",				lib_debug)
	shutil.copy(obj_dir + "avmplus/Release_Debugger/avmplus.lib",	lib_release + "avmplus_debugger.lib")
	shutil.copy(obj_dir + "avmplus/Debug_Debugger/avmplus.lib",		lib_debug + "avmplus_debugger.lib")

	# eval
	shutil.copy(obj_dir + "eval/Release/eval.lib",				lib_release)
	shutil.copy(obj_dir + "eval/Debug/eval.lib",				lib_debug)
	shutil.copy(obj_dir + "eval/Release_Debugger/eval.lib",		lib_release + "eval_debugger.lib")
	shutil.copy(obj_dir + "eval/Debug_Debugger/eval.lib",		lib_debug + "eval_debugger.lib")

	# MMgc
	shutil.copy(obj_dir + "MMgc/Release/MMgc.lib",			lib_release + "MMgc.lib")
	shutil.copy(obj_dir + "MMgc/Debug/MMgc_d.lib",			lib_debug + "MMgc.lib")
	shutil.copy(obj_dir + "MMgc/Release_Debugger/MMgc.lib",	lib_release + "MMgc_debugger.lib")
	shutil.copy(obj_dir + "MMgc/Debug_Debugger/MMgc_d.lib",	lib_debug + "MMgc_debugger.lib")

	# nanojit
	shutil.copy(obj_dir + "nanojit/Release/nanojit.lib",			lib_release)
	shutil.copy(obj_dir + "nanojit/Debug/nanojit.lib",				lib_debug)
	shutil.copy(obj_dir + "nanojit/Release_Debugger/nanojit.lib",	lib_release + "nanojit_debugger.lib")
	shutil.copy(obj_dir + "nanojit/Debug_Debugger/nanojit.lib",		lib_debug + "nanojit_debugger.lib")

def copy_tamarin_libs_vc10():

	obj_dir = "../tamarin/platform/win32/obj2010/"
	lib_release = "vc10/caspin/lib/Release/"
	lib_debug = "vc10/caspin/lib/Debug/"

	shutil.copy(obj_dir + "Release/avm.lib",			lib_release)
	shutil.copy(obj_dir + "Debug/avm.lib",				lib_debug)
	shutil.copy(obj_dir + "Release_Debugger/avm.lib",	lib_release + "avm_debugger.lib")
	shutil.copy(obj_dir + "Debug_Debugger/avm.lib",		lib_debug + "avm_debugger.lib")

# --------------------------------------
# ---------------- MAIN ----------------
# --------------------------------------
target = sys.argv[1]

create_dirs(target)

print "copying caspin headers"
files.copy("../include/*.h", target + "/caspin/include/")

print "copying caspin libraries"
files.copy("../lib/Release/*.lib", target + "/caspin/lib/Release/")
files.copy("../lib/Debug/*.lib", target + "/caspin/lib/Debug/")

print "copying caspin tools"
shutil.copy("../tools/cspcompile.py", target + "/caspin/tools/")
shutil.copy("../tools/cspgenerate.py", target + "/caspin/tools/")

print "copying tamarin headers"
copy_tamarin_headers(target)

print "copying tamarin libraries"
if target == "vc9":
	copy_tamarin_libs_vc9()
elif target == "vc10":
	copy_tamarin_libs_vc10()

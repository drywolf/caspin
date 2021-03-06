set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

include (build_tools)

#-----------------------------------------------------------------------
# PREPROCESSOR SETTINGS
#-----------------------------------------------------------------------

# unix g++ flags
if(UNIX)
	set (CMAKE_CXX_FLAGS ${CASPIN_CXX_FLAGS})
endif()

# generic compiler switches
add_definitions (-DAVMSHELL_BUILD)

# unix compiler switches
if(UNIX)
	add_definitions (-DUNIX -DAVMPLUS_UNIX -DSOFT_ASSERTS)
endif()

# disable win32 warnings
if(WIN32)
	add_definitions("/wd4291 /wd4996")
endif()

#-----------------------------------------------------------------------
# INCLUDE DIRECTORIES & SOURCE FILES
#-----------------------------------------------------------------------

# caspin and tamarin include directories
set (include_dirs
	../include
	${CMAKE_SOURCE_DIR}/include
	${tamarin_include_dirs}
)

# read the source files list
include (test_cpp_interface_src_files)

#-----------------------------------------------------------------------
# BUILD SETTINGS & COMPILATION
#-----------------------------------------------------------------------

if(WIN32)
	# link statically against the MSVCRT
	link_static_crt()
endif(WIN32)

# build output directory
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/dev/tests/cpp_interface/bin)

# set the include directories
include_directories (${include_dirs})

# unix fix for the tamarin libraries
if(UNIX)
	link_libraries (${tamarin_debugger_libs})
endif()

# add the sample
add_executable (test_cpp_interface ${file_root})

# automatically update the source files list
auto_rebuild_src_files (test_cpp_interface)

# the path that contains the AS3 files for this sample
set (as3_dir ${CMAKE_SOURCE_DIR}/dev/tests/cpp_interface/as3)

# automatically compile the sample AS3 files to ABC if necessary
add_custom_command (
	OUTPUT ${as3_dir}/test_cpp_interface.abc
	MAIN_DEPENDENCY ${as3_dir}/test_cpp_interface.as
	WORKING_DIRECTORY ${as3_dir}
	COMMAND python ${as3_dir}/build.py
)

# preprocessor macro to enable the debugger
set_property (TARGET test_cpp_interface PROPERTY COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} CSP_USE_RUNTIME_DEBUGGER)
	
# link the debugger libraries
target_link_libraries (test_cpp_interface caspin_debugger)

# create the "run_test.py" script
set (run_test_py ${CMAKE_CURRENT_SOURCE_DIR}/../run_test.py)

get_target_property (test_binary test_cpp_interface DEBUG_LOCATION)
file (WRITE ${run_test_py} "import os\n\n")
file (APPEND ${run_test_py} "os.system(\"${test_binary} avm_class > test_result.txt\")\n")
file (APPEND ${run_test_py} "os.system(\"${test_binary} avm_string >> test_result.txt\")\n")
file (APPEND ${run_test_py} "os.system(\"${test_binary} csp_string >> test_result.txt\")\n")

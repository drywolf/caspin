set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

include (caspin_globals)

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
	${CMAKE_SOURCE_DIR}/include
	${tamarin_include_dirs}
)

# load the source-groups & file list
include (caspin_src_files)

# on unix we need to build "eval" ourselves
if(UNIX)
	file (GLOB eval_src "${tamarin_path}/eval/*.cpp")
	set (file_root ${file_root} ${eval_src})
endif()

#-----------------------------------------------------------------------
# BUILD SETTINGS & COMPILATION
#-----------------------------------------------------------------------

# build output directory
set (LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# set the include directories
include_directories (${include_dirs})

# add the caspin library
add_library (caspin STATIC ${file_root})

# add the caspin debugger library (tamarin runtime debugger enabled)
add_library (caspin_debugger STATIC ${file_root})

# automatically update the source-file list with folders4cmake
auto_rebuild_src_files (caspin ${tamarin_path} \${tamarin_path})

# the path that contains the caspin_base package AS3 files
set (as3_dir ${CMAKE_SOURCE_DIR}/as3)

# automatically compile the caspin_base AS3 files to ABC if necessary
add_custom_command (
	OUTPUT ${CMAKE_SOURCE_DIR}/include/caspin_base.h ${CMAKE_SOURCE_DIR}/src/caspin_base.cpp
	MAIN_DEPENDENCY ${as3_dir}/caspin_base.as
	WORKING_DIRECTORY ${as3_dir}
	COMMAND python ${as3_dir}/build.py
)

# enable the tamarin debugger for the caspin debugger library build
set_property (TARGET caspin_debugger PROPERTY COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} DEBUGGER CSP_USE_RUNTIME_DEBUGGER)

# link the necessary libraries
target_link_libraries (caspin ${tamarin_libs})
target_link_libraries (caspin_debugger ${tamarin_debugger_libs})

# write the cmake import file
set (caspin_import_file ${CMAKE_SOURCE_DIR}/cmake/caspin_import_${CMAKE_GENERATOR}.cmake)
export (TARGETS caspin caspin_debugger FILE ${caspin_import_file})
file (APPEND ${caspin_import_file} "\n# caspin include directories\n")
file (APPEND ${caspin_import_file} "set (caspin_include_dirs ${include_dirs})\n")

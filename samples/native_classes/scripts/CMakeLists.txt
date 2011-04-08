set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

include (build_tools)

#-----------------------------------------------------------------------
# CMAKE OPTIONS
#-----------------------------------------------------------------------
option(sample_NATIVE_CLASSES_use_runtime_debugger "Use runtime debugger for the native classes sample" OFF)

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
	../../../include
	${tamarin_include_dirs}
)

# read the source files list
include (native_classes_src_files)

#-----------------------------------------------------------------------
# BUILD SETTINGS & COMPILATION
#-----------------------------------------------------------------------

if(WIN32)
	# link statically against the MSVCRT
	link_static_crt()
endif(WIN32)

# build output directory
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# set the include directories
include_directories (${include_dirs})

# unix fix for the tamarin libraries
if(UNIX)

	if(sample_NATIVE_CLASSES_use_runtime_debugger)
	
		link_libraries (${tamarin_debugger_libs})
	
	else(sample_NATIVE_CLASSES_use_runtime_debugger)
	
		link_libraries (${tamarin_libs})
	
	endif(sample_NATIVE_CLASSES_use_runtime_debugger)

endif()

# add the sample
add_executable (native_classes ${file_root})

# automatically update the source files list
auto_rebuild_src_files (native_classes)

# the sample path
set (sample_dir ${CMAKE_SOURCE_DIR}/samples/native_classes)

# the path that contains the AS3 files for this sample
set (as3_dir ${sample_dir}/as3)

# automatically compile the native classes to ABC and generate the C++ glue code
add_custom_command (
	OUTPUT ${as3_dir}/myclasses.abc ${sample_dir}/include/myclasses.h ${sample_dir}/src/myclasses.cpp
	MAIN_DEPENDENCY ${as3_dir}/myclasses.as
	WORKING_DIRECTORY ${as3_dir}
	COMMAND python ${as3_dir}/build_myclasses.py
)

# automatically compile the sample AS3 files to ABC if necessary
add_custom_command (
	OUTPUT ${as3_dir}/playpen.abc
	MAIN_DEPENDENCY ${as3_dir}/playpen.as
	DEPENDS ${as3_dir}/myclasses.abc
	WORKING_DIRECTORY ${as3_dir}
	COMMAND python ${as3_dir}/build_playpen.py
)

# enable the tamarin debugger
if(sample_NATIVE_CLASSES_use_runtime_debugger)
	
	# preprocessor macro to enable the debugger
	set_property (TARGET native_classes PROPERTY COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} CSP_USE_RUNTIME_DEBUGGER)
	
	# link the debugger libraries
	target_link_libraries (native_classes caspin_debugger)
	
else(sample_NATIVE_CLASSES_use_runtime_debugger)
	
	# link the default libraries
	target_link_libraries (native_classes caspin)
	
endif(sample_NATIVE_CLASSES_use_runtime_debugger)

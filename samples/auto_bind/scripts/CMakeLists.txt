set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------
# CMAKE OPTIONS
#-----------------------------------------------------------------------
option(sample_AUTO_BIND_use_runtime_debugger "Use runtime debugger for the auto_bind sample" OFF)

#-----------------------------------------------------------------------
# PREPROCESSOR SETTINGS
#-----------------------------------------------------------------------

# unix g++ flags
if(UNIX)
	set (CMAKE_CXX_FLAGS "-O -Wcast-align -Wdisabled-optimization -Wextra -Wformat=2 -Winit-self -Winvalid-pch -Wno-invalid-offsetof -Wno-switch -Wparentheses -Wpointer-arith -Wreorder -Wsign-compare -Wwrite-strings -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wno-char-subscripts -fmessage-length=0 -fno-exceptions -fno-rtti -fno-check-new -fstrict-aliasing -fsigned-char -Wstrict-null-sentinel -Werror -Wempty-body -Wno-logical-op -Wmissing-field-initializers -Wstrict-aliasing=3 -Wno-array-bounds -Wno-clobbered -Wstrict-overflow=0 -funit-at-a-time -Wuninitialized")
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
include (auto_bind_src_files)

#-----------------------------------------------------------------------
# BUILD SETTINGS & COMPILATION
#-----------------------------------------------------------------------

# build output directory
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# set the include directories
include_directories (${include_dirs})

# unix fix for the tamarin libraries
if(UNIX)
#{
	if(sample_AUTO_BIND_use_runtime_debugger)
#	{
		link_libraries (${tamarin_debugger_libs})
#	}
	else(sample_AUTO_BIND_use_runtime_debugger)
#	{
		link_libraries (${tamarin_libs})
#	}
	endif(sample_AUTO_BIND_use_runtime_debugger)
#}
endif()

# add the sample
add_executable (auto_bind ${file_root})

add_custom_command (
	OUTPUT ${as3_path}/MyClass.as
	DEPENDS ../include/MyClass.h
	COMMAND java -jar C:/as3_bind.jar ${CMAKE_CURRENT_SOURCE_DIR}/../include/MyClass.h ${as3_path}/MyClass.as
)

# the path that contains the AS3 files for this sample
set (as3_dir ${CMAKE_SOURCE_DIR}/samples/auto_bind/as3)

# automatically compile the AS3 files to ABC if necessary
add_custom_command (
	OUTPUT ${as3_path}/playpen.abc ${as3_path}/auto_classes.abc
	DEPENDS ${as3_path}/playpen.as ${as3_path}/MyClass.as
	WORKING_DIRECTORY ${as3_path}
	COMMAND ${as3_path}/build.py
)

auto_rebuild_src_files (auto_bind)

# enable the tamarin debugger
if(sample_AUTO_BIND_use_runtime_debugger)
#{
	# preprocessor macro to enable the debugger
	set_property (TARGET auto_bind PROPERTY COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} CSP_USE_RUNTIME_DEBUGGER)
	
	# link the debugger libraries
	target_link_libraries (auto_bind caspin_debugger)
#}
else(sample_AUTO_BIND_use_runtime_debugger)
#{
	# link the default libraries
	target_link_libraries (auto_bind caspin)
#}
endif(sample_AUTO_BIND_use_runtime_debugger)

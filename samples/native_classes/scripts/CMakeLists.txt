set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

#-----------------------------------------------------------------------
# CMAKE OPTIONS
#-----------------------------------------------------------------------
option(sample_NATIVE_CLASSES_use_runtime_debugger "Use runtime debugger for the native classes sample" OFF)

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
include (source_files)

#-----------------------------------------------------------------------
# BUILD SETTINGS & COMPILATION
#-----------------------------------------------------------------------

# build output directory
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# set the include directories
include_directories (${include_dirs})

# unix fix for the tamarin libraries
if(UNIX)

	if(sample_MINIMAL_use_runtime_debugger)
	
		link_libraries (${tamarin_debugger_libs})
	
	else(sample_MINIMAL_use_runtime_debugger)
	
		link_libraries (${tamarin_libs})
	
	endif(sample_MINIMAL_use_runtime_debugger)

endif()

# add the sample
add_executable (native_classes ${file_root})

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

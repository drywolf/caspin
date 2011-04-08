cmake_minimum_required (VERSION 2.6)
project (caspin)

#-----------------------------------------------------------------------
# CMAKE OPTIONS
#-----------------------------------------------------------------------
option(BUILD_UNIT_TESTS "Generate build files for the development test tools" OFF)
option(BUILD_ONLY_DEBUG_RELEASE "Generate only Debug and Release configurations (exclude RelWithDebInfo and MinSizeRel)" ON)

# set up the module path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# remove the MinSizeRel and RelWithDebInfo configurations
if(BUILD_ONLY_DEBUG_RELEASE)
	set (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)
endif(BUILD_ONLY_DEBUG_RELEASE)

# load the folders4cmake utils
set (folders4cmake_path ${CMAKE_SOURCE_DIR}/cmake/folders4cmake.jar)
include (folders4cmake)

# dependency discovery
find_package (tamarin REQUIRED)

add_subdirectory (scripts)
add_subdirectory (samples/minimal/scripts)
add_subdirectory (samples/native_classes/scripts)

# generate test build files only if requested
if(BUILD_UNIT_TESTS)
#{
	add_subdirectory (dev/tests/cpp_interface/scripts)
#}
endif(BUILD_UNIT_TESTS)

cmake_minimum_required (VERSION 2.6)
project (caspin)

# set up the module path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# remove the MinSizeRel and RelWithDebInfo configurations
set (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)

# load the folders4cmake utils
set (folders4cmake_path ${CMAKE_SOURCE_DIR}/cmake/folders4cmake.jar)
include (folders4cmake)

# dependency discovery
find_package (tamarin REQUIRED)

add_subdirectory (scripts)
add_subdirectory (samples/minimal/scripts)
add_subdirectory (samples/native_classes/scripts)

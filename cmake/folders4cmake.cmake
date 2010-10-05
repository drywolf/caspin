#-----------------------------------------------------------------------------
# This source file is part of "folders4cmake"
# (a small java application to generate CMake source_groups from Visual Studio 9/10 project files)
# For the latest info, see http://www.fuse-software.com/
# 
# Copyright (c) 2009-2010 Fuse-Software (tm)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# a simple function that automatically forces CMake to regenerate the
# f4cmake source files list, if the corresponding IDE project file has been altered
#-----------------------------------------------------------------------------
function (auto_rebuild_src_files target_project)
#{
	if(WIN32)
#	{
		if(EXISTS ${folders4cmake_path})
#		{
			if(MSVC90)
				set (target_filename ${target_project}.vcproj)
			endif(MSVC90)
			
			if(MSVC10)
				# CMake's custom_commands don't work very well with MSVC10 yet
				# therefore we disable the automatic rebuild of source-file lists here
				return()
				set (target_filename ${target_project}.vcxproj.filters)
			endif(MSVC10)
			
			set (vcproj_path ${CMAKE_CURRENT_BINARY_DIR}/${target_filename})
			set (source_file_path ${CMAKE_CURRENT_SOURCE_DIR}/${target_project}_src_files.cmake)
			
			add_custom_command (
				OUTPUT ${source_file_path} 
				DEPENDS ${vcproj_path} 
				COMMAND java -jar ${folders4cmake_path} 
				ARGS ${vcproj_path} ${source_file_path} ${ARGN}
			)
#		}
		else(EXISTS ${folders4cmake_path})
#		{
			message ("Warning: \${folders4cmake_path} has not been set or is invalid, ignoring call to \"auto_rebuild_src_files(${target_project})\"")
#		}
		endif(EXISTS ${folders4cmake_path})
#	}
	endif(WIN32)
#}
endfunction (auto_rebuild_src_files)
#-----------------------------------------------------------------------------

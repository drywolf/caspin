# top-level files
set (file_root
	../include/cspPrerequisites.h
)

# as3 files
set (file_root_as3
	../as3/build.py
	../as3/caspin_base.as
)

# caspin files
set (file_root_caspin
	../include/cspNativePackage.h
	../include/cspOutputListener.h
	../include/cspOutputLogger.h
	../include/cspScriptDefinition.h
	../include/cspSystem.h
	../include/cspVmCore.h
	../src/cspOutputLogger.cpp
	../src/cspSystem.cpp
	../src/cspVmCore.cpp
)

# caspin -> [exclude] files
set (file_root_caspin_exclude
	../include/caspin_base.h
	../src/caspin_base.cpp
)

# cmake files
set (file_root_cmake
	caspin_src_files.cmake
	caspin_src_files.cmake.rule
	CMakeLists.txt
)

# platforms files
set (file_root_platforms
	../include/cspUnixPlatform.h
	../include/cspWinPlatform.h
)

# tamarin files
set (file_root_tamarin
	${tamarin_path}/shell/DebugCLI.cpp
	${tamarin_path}/shell/DebugCLI.h
	${tamarin_path}/shell/FileInputStream.cpp
	${tamarin_path}/shell/FileInputStream.h
	${tamarin_path}/VMPI/AvmAssert.cpp
	${tamarin_path}/VMPI/AvmAssert.h
)

# tamarin -> [unix] files
set (file_root_tamarin_unix
	${tamarin_path}/shell/PosixFile.cpp
	${tamarin_path}/shell/PosixFile.h
	${tamarin_path}/shell/PosixPartialPlatform.cpp
	${tamarin_path}/shell/PosixPartialPlatform.h
	${tamarin_path}/VMPI/MMgcPortUnix.cpp
	${tamarin_path}/VMPI/PosixPortUtils.cpp
	${tamarin_path}/VMPI/ThreadsPosix.cpp
	${tamarin_path}/VMPI/UnixDebugUtils.cpp
)

# tamarin -> [win32] files
set (file_root_tamarin_win32
	${tamarin_path}/platform/win32/win32cpuid.cpp
	${tamarin_path}/shell/WinFile.cpp
	${tamarin_path}/shell/WinFile.h
	${tamarin_path}/VMPI/WinDebugUtils.cpp
	${tamarin_path}/VMPI/WinPortUtils.cpp
)

# tools files
set (file_root_tools
	../tools/cspcompile.py
	../tools/cspgenerate.py
)

# the source groups
source_group(""                  FILES ${file_root})
source_group("as3"               FILES ${file_root_as3})
source_group("caspin"            FILES ${file_root_caspin})
source_group("caspin\\[exclude]" FILES ${file_root_caspin_exclude})
source_group("cmake"             FILES ${file_root_cmake})
source_group("platforms"         FILES ${file_root_platforms})
source_group("tamarin"           FILES ${file_root_tamarin})
source_group("tamarin\\[unix]"   FILES ${file_root_tamarin_unix})
source_group("tamarin\\[win32]"  FILES ${file_root_tamarin_win32})
source_group("tools"             FILES ${file_root_tools})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_as3}
	${file_root_caspin}
	${file_root_caspin_exclude}
	${file_root_cmake}
	${file_root_platforms}
	${file_root_tamarin}
	${file_root_tamarin_unix}
	${file_root_tamarin_win32}
	${file_root_tools}
)

# exclude certain files from all builds
set_source_files_properties (${file_root_caspin_exclude} PROPERTIES HEADER_FILE_ONLY TRUE)

# exclude all non-UNIX files from UNIX builds
if (UNIX)
#{
	set_source_files_properties (${file_root_caspin_exclude} PROPERTIES HEADER_FILE_ONLY TRUE)
	set_source_files_properties (${file_root_tamarin_win32} PROPERTIES HEADER_FILE_ONLY TRUE)
#}
endif (UNIX)

# exclude all non-WIN32 files from WIN32 builds
if (WIN32)
#{
	set_source_files_properties (${file_root_caspin_exclude} PROPERTIES HEADER_FILE_ONLY TRUE)
	set_source_files_properties (${file_root_tamarin_unix} PROPERTIES HEADER_FILE_ONLY TRUE)
#}
endif (WIN32)

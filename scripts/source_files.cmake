# top-level files
set (file_root
	../include/cspPrerequisites.h
)

# ActionScript 3 files
set (file_root_as3
	../as3/caspin_base.as
)

# caspin source files
set (file_root_caspin
	../include/cspNativePackage.h
	../include/cspOutputListener.h
	../include/cspOutputLogger.h
	../include/cspSystem.h
	../include/cspVmCore.h
	../src/cspOutputLogger.cpp
	../src/cspSystem.cpp
	../src/cspVmCore.cpp
)

# platform specific implementations
set (file_root_platforms
	../include/cspWinPlatform.h
	../include/cspUnixPlatform.h
)

# platform independent tamarin files
set (file_root_tamarin
	${tamarin_path}/VMPI/AvmAssert.h
	${tamarin_path}/VMPI/AvmAssert.cpp
	${tamarin_path}/shell/DebugCLI.h
	${tamarin_path}/shell/DebugCLI.cpp
	${tamarin_path}/extensions/DictionaryGlue.h
	${tamarin_path}/extensions/DictionaryGlue.cpp
	${tamarin_path}/shell/FileInputStream.h
	${tamarin_path}/shell/FileInputStream.cpp
)

# unix specific tamarin source files
if(UNIX)
	set (file_root_tamarin ${file_root_tamarin}
		${tamarin_path}/shell/PosixPartialPlatform.cpp
		${tamarin_path}/shell/PosixFile.cpp
		${tamarin_path}/VMPI/PosixPortUtils.cpp
		${tamarin_path}/VMPI/MMgcPortUnix.cpp
		${tamarin_path}/VMPI/ThreadsPosix.cpp
	)
endif()

# win32 specific tamarin source files
if(WIN32)
	set (file_root_tamarin ${file_root_tamarin}
		${tamarin_path}/platform/win32/win32cpuid.cpp
		${tamarin_path}/shell/WinFile.h
		${tamarin_path}/shell/WinFile.cpp
		${tamarin_path}/VMPI/WinDebugUtils.cpp
		${tamarin_path}/VMPI/WinPortUtils.cpp
	)
endif()

# the source groups
source_group(""			FILES ${file_root})
source_group(as3		FILES ${file_root_as3})
source_group(caspin		FILES ${file_root_caspin})
source_group(platforms	FILES ${file_root_platforms})
source_group(tamarin	FILES ${file_root_tamarin})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_as3}
	${file_root_caspin}
	${file_root_platforms}
	${file_root_tamarin}
)

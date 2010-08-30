# top-level files
set (file_root
	../src/minimal_main.cpp
)

# as3 files
set (file_root_as3
	../as3/minimal.as
)

# the source groups
source_group(""    FILES ${file_root})
source_group("as3" FILES ${file_root_as3})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_as3}
)

# top-level files
set (file_root
	../include/MyClass.h
	../src/MyClass.cpp
	../src/native_classes_main.cpp
)

# as3 files
set (file_root_as3
	../as3/myclasses.as
	../as3/playpen.as
)

# the source groups
source_group(""    FILES ${file_root})
source_group("as3" FILES ${file_root_as3})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_as3}
)

# top-level files
set (file_root
)

# as3 files
set (file_root_as3
	../as3/myclasses.abc
	../as3/myclasses.as
	../as3/playpen.abc
	../as3/playpen.as
)

# cmake files
set (file_root_cmake
	../as3/myclasses.abc.rule
	../as3/playpen.abc.rule
	CMakeLists.txt
	native_classes_src_files.cmake
	native_classes_src_files.cmake.rule
)

# cpp files
set (file_root_cpp
	../include/MyClass.h
	../src/MyClass.cpp
	../src/native_classes_main.cpp
)

# cpp -> [exclude] files
set (file_root_cpp_exclude
	../include/myclasses.h
	../src/myclasses.cpp
)

# the source groups
source_group(""               FILES ${file_root})
source_group("as3"            FILES ${file_root_as3})
source_group("cmake"          FILES ${file_root_cmake})
source_group("cpp"            FILES ${file_root_cpp})
source_group("cpp\\[exclude]" FILES ${file_root_cpp_exclude})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_as3}
	${file_root_cmake}
	${file_root_cpp}
	${file_root_cpp_exclude}
)

# exclude certain files from all builds
set_source_files_properties (${file_root_cpp_exclude} PROPERTIES HEADER_FILE_ONLY TRUE)

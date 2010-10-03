# top-level files
set (file_root
)

# abc files
set (file_root_abc
	../as3/auto_classes.abc
	../as3/playpen.abc
)

# as3 files
set (file_root_as3
	../as3/build.py
	../as3/MyClass.as
	../as3/playpen.as
)

# cmake files
set (file_root_cmake
	../as3/MyClass.as.rule
	../as3/playpen.abc.rule
	auto_bind_src_files.cmake
	auto_bind_src_files.cmake.rule
	CMakeLists.txt
)

# cpp files
set (file_root_cpp
	../src/auto_bind_main.cpp
)

# cpp -> [exclude] files
set (file_root_cpp_exclude
	../include/auto_classes.h
	../include/MyClass.h
	../src/auto_classes.cpp
	../src/MyClass.cpp
)

# the source groups
source_group(""               FILES ${file_root})
source_group("abc"            FILES ${file_root_abc})
source_group("as3"            FILES ${file_root_as3})
source_group("cmake"          FILES ${file_root_cmake})
source_group("cpp"            FILES ${file_root_cpp})
source_group("cpp\\[exclude]" FILES ${file_root_cpp_exclude})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_abc}
	${file_root_as3}
	${file_root_cmake}
	${file_root_cpp}
	${file_root_cpp_exclude}
)

# exclude certain files from all builds
set_source_files_properties (${file_root_cpp_exclude} PROPERTIES HEADER_FILE_ONLY TRUE)

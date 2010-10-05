# top-level files
set (file_root
)

# as3 files
set (file_root_as3
	../as3/build.py
	../as3/minimal.abc
	../as3/minimal.as
)

# cmake files
set (file_root_cmake
	../as3/minimal.abc.rule
	CMakeLists.txt
	minimal_src_files.cmake
	minimal_src_files.cmake.rule
)

# cpp files
set (file_root_cpp
	../src/minimal_main.cpp
)

# the source groups
source_group(""      FILES ${file_root})
source_group("as3"   FILES ${file_root_as3})
source_group("cmake" FILES ${file_root_cmake})
source_group("cpp"   FILES ${file_root_cpp})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_as3}
	${file_root_cmake}
	${file_root_cpp}
)

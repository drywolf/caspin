# top-level files
set (file_root
)

# cmake files
set (file_root_cmake
	CMakeLists.txt
	test_cpp_interface_src_files.cmake
	test_cpp_interface_src_files.cmake.rule
)

# the source groups
source_group(""      FILES ${file_root})
source_group("cmake" FILES ${file_root_cmake})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_cmake}
)

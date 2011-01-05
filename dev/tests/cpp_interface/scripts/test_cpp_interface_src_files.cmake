# top-level files
set (file_root
)

# as3 files
set (file_root_as3
	../as3/build.py
	../as3/test_cpp_interface.abc
	../as3/test_cpp_interface.as
)

# cmake files
set (file_root_cmake
	../as3/test_cpp_interface.abc.rule
	CMakeLists.txt
	test_cpp_interface_src_files.cmake
	test_cpp_interface_src_files.cmake.rule
)

# cpp files
set (file_root_cpp
	../include/test_base.h
	../src/test_cpp_interface.cpp
)

# cpp -> tests files
set (file_root_cpp_tests
	../include/test_avm_class_methods.h
	../include/test_avm_string_methods.h
	../include/test_csp_string_methods.h
)

# the source groups
source_group(""           FILES ${file_root})
source_group("as3"        FILES ${file_root_as3})
source_group("cmake"      FILES ${file_root_cmake})
source_group("cpp"        FILES ${file_root_cpp})
source_group("cpp\\tests" FILES ${file_root_cpp_tests})

# append all files to the file_root
set (file_root
	${file_root}
	${file_root_as3}
	${file_root_cmake}
	${file_root_cpp}
	${file_root_cpp_tests}
)

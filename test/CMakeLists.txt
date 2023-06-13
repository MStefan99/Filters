set(TEST_DIRECTORY .)

link_libraries(FixMath)
add_compile_options(-DTEST)

if (${CMAKE_CXX_COMPILER} MATCHES "clang-cl")
	add_compile_options(/EHs)
endif ()


# Including Tester library
add_subdirectory(${TEST_DIRECTORY}/Tester)
link_libraries(Tester)


# Individual test targets
add_executable(
		Test_fixed_point_math
		${TEST_DIRECTORY}/tests/fixed.test.cpp
)

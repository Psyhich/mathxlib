find_package(GTest CONFIG REQUIRED)

include(GoogleTest)

add_executable(matrix_tests
	src/unittests_matrix.cpp
)
target_link_libraries(matrix_tests
	PRIVATE
		matrixlib
)
target_link_system_libraries(matrix_tests
	PRIVATE
		GTest::gtest_main
		GTest::gmock
)
target_disable_clang_tidy(matrix_tests)

gtest_discover_tests(matrix_tests)

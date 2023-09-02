cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(ENABLE_DEVELOPER_MODE "Enable 'developer mode'" TRUE)
option(ENABLE_TESTING "Enable the tests" ${PROJECT_IS_TOP_LEVEL})

project(mathxlib
	VERSION 0.1.0
	DESCRIPTION "Simple library for mathematics"
	HOMEPAGE_URL "https://github.com/Psyhich/mathxlib"
	LANGUAGES CXX C
)

set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/load_project_options.cmake")
include(cmake/CPM.cmake)

if(ENABLE_DEVELOPER_MODE)
	set(ENABLE_CLANG_TIDY ENABLE_CLANG_TIDY ON)
	set(ENABLE_CPPCHECK ENABLE_CPPCHECK ON)
	if(ENABLE_TESTING)
		set(ENABLE_COVERAGE ENABLE_COVERAGE ON)
	endif()

	check_sanitizers_support(
		ENABLE_SANITIZER_ADDRESS
		ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
		ENABLE_SANITIZER_LEAK
		ENABLE_SANITIZER_THREAD
		ENABLE_SANITIZER_MEMORY
	)
endif()

project_options(
	PREFIX "mathxlib"
	ENABLE_CACHE

	ENABLE_STACK_PROTECTION ON
	ENABLE_CONTROL_FLOW_PROTECTION ON
	ENABLE_OVERFLOW_PROTECTION ON

	${ENABLE_CLANG_TIDY}
	${ENABLE_CPPCHECK}
	${ENABLE_COVERAGE}
	${ENABLE_CONTROL_FLOW_PROTECTION}
	${ENABLE_STACK_PROTECTION}
	${ENABLE_OVERFLOW_PROTECTION}

	${ENABLE_SANITIZER_ADDRESS}
	${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
	${ENABLE_SANITIZER_THREAD}
	${ENABLE_SANITIZER_MEMORY}
)

CPMAddPackage("gh:fmtlib/fmt#10.1.0")

if(ENABLE_TESTING)
	CPMAddPackage("gh:google/googletest@1.14.0")
	add_executable(unit_tests)
	target_disable_clang_tidy(unit_tests)
	target_disable_cpp_check(unit_tests)

	target_link_system_libraries(unit_tests
		PRIVATE
			gtest_main
			gmock
			${CMAKE_DL_LIBS}
	)
	include(GoogleTest)
	include(cmake/CodeCoverage.cmake)

	enable_testing()


	setup_target_for_coverage_gcovr_xml(
		NAME generate_report
		EXECUTABLE unit_tests
		DEPENDENCIES unit_tests 
		BASE_DIRECTORY "${CMAKE_SOURCE_DIR}/"
		EXCLUDE
			"unittest_*"
		LCOV_ARGS
			"--no-external"
	)
	setup_target_for_coverage_lcov(
		NAME generate_html_report
		EXECUTABLE unit_tests
		DEPENDENCIES unit_tests 
		BASE_DIRECTORY "${CMAKE_SOURCE_DIR}/"
		EXCLUDE
			"unittest_*"
		LCOV_ARGS
			"--no-external"
	)
endif()

add_subdirectory(configured_files)
add_subdirectory(matrixes)

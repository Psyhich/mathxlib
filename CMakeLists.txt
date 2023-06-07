cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(ENABLE_DEVELOPER_MODE TRUE CACHE BOOL "Enable 'developer mode'")
set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)

option(ENABLE_TESTING "Enable the tests" ${PROJECT_IS_TOP_LEVEL})

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
	"${CMAKE_CURRENT_BINARY_DIR}/")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_BINARY_DIR}/")

project(matrixlib
	VERSION 0.0.6
	DESCRIPTION "Simple library for manipulations with matrixes"
	HOMEPAGE_URL ""
	LANGUAGES CXX C
)

set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

get_property(BUILDING_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(BUILDING_MULTI_CONFIG)
	if(NOT CMAKE_BUILD_TYPE)
		message(TRACE "Setting up multi-config build types")
		set(CMAKE_CONFIGURATION_TYPES
				Debug
				Release
				RelWithDebInfo
				MinSizeRel
			CACHE STRING "Enabled build types" FORCE
		)
	else()
		message(TRACE "User chose a specific build type, so we are using that")
		set(CMAKE_CONFIGURATION_TYPES
				${CMAKE_BUILD_TYPE}
			CACHE STRING "Enabled build types" FORCE
		)
	endif()
endif()

include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.29.0.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)
include(${_project_options_SOURCE_DIR}/src/DynamicProjectOptions.cmake)

if(ENABLE_TESTING)
	set(CONAN_TESTS_DEPENDENCIES "matrixlib:build_tests=True")
endif()

dynamic_project_options(
	ENABLE_CONAN
	CONAN_OPTIONS
		${CONAN_TESTS_DEPENDENCIES}
	CPPCHECK_OPTIONS
		--enable=style,performance,warning,portability
		--inline-suppr
		--suppress=cppcheckError
		--suppress=internalAstError
		--suppress=unmatchedSuppression
		--suppress=passedByValue
		--suppress=syntaxError
		--inconclusive
)

target_compile_features(project_options
	INTERFACE
		cxx_std_${CMAKE_CXX_STANDARD}
)

add_library(matrixlib::project_options INTERFACE IMPORTED)
add_library(matrixlib::project_warnings INTERFACE IMPORTED)

add_subdirectory(configured_files)

if(ENABLE_TESTING)
	include(CodeCoverage)
	append_coverage_compiler_flags()
endif()
add_subdirectory(matrixes)

if(ENABLE_TESTING)
	enable_testing()
	add_subdirectory(unit_tests)
endif()

package_project(
	TARGETS 
		matrixlib
		project_options
		project_warnings
	PUBLIC_INCLUDES "matrixes/include"
)
include(CPack)

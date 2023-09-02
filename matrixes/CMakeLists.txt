add_library(matrixes OBJECT
	src/matrixes.cpp
	src/algorithms.cpp
)

target_link_libraries(matrixes
	PUBLIC
		fmt
	PRIVATE
		mathxlib_project_options
		mathxlib_project_warnings
)
target_include_directories(matrixes 
	PUBLIC
		include
	PRIVATE
		"${CMAKE_BINARY_DIR}/configured_files/include"
)

if(ENABLE_TESTING)
	add_subdirectory(unit_tests)
endif()

add_library(mathx_matrixes
	src/matrixes.cpp
)

target_link_libraries(mathx_matrixes
	PUBLIC
		fmt
	PRIVATE
		mathxlib_project_options
		mathxlib_project_warnings
)
target_include_directories(mathx_matrixes 
	PUBLIC
		include
	PRIVATE
		"${CMAKE_BINARY_DIR}/configured_files/include"
)

if(ENABLE_TESTING)
	add_subdirectory(unit_tests)
endif()

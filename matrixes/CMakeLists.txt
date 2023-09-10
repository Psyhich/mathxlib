add_library(mathx_matrixes INTERFACE)

target_link_libraries(mathx_matrixes
	INTERFACE
		fmt
)

if (ENABLE_DEVELOPER_MODE)
	target_link_libraries(mathx_matrixes
		INTERFACE
			mathxlib_project_options
			mathxlib_project_warnings
	)
endif()

target_include_directories(mathx_matrixes 
	INTERFACE
		include

		"${CMAKE_BINARY_DIR}/configured_files/include"
)

if(ENABLE_TESTING)
	add_subdirectory(unit_tests)
endif()

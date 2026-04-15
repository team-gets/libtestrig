if (WIN32)
	add_library(EposCmd SHARED IMPORTED)
	set_target_properties(EposCmd PROPERTIES
		IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/bin/EposCmd64.dll"
		IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/bin/EposCmd64.lib")

	set(EposCmd-FOUND TRUE)
else()
	add_library(EposCmd SHARED IMPORTED)
	set_target_properties(EposCmd PROPERTIES
		IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/bin/libEposCmd.so"
		IMPORTED_IMPLIB "")

	set(EposCmd-FOUND TRUE)
endif()

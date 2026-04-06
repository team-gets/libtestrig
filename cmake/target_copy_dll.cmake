function(target_copy_dll IN_TARGET)
	if (WIN32)
		add_custom_command(TARGET ${IN_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_RUNTIME_DLLS:${IN_TARGET}> $<TARGET_FILE_DIR:${IN_TARGET}>
		COMMAND_EXPAND_LISTS)
	endif()
endfunction(target_copy_dll IN_TARGET)

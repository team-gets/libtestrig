ENABLE_TESTING()

add_executable(sock_name_exe "${CMAKE_CURRENT_LIST_DIR}/test_sock_name.c")
target_link_libraries(sock_name_exe PRIVATE testrig)
add_test(NAME sockname COMMAND sock_name_exe)

if (WIN32)
	add_executable(test_mgr "${CMAKE_CURRENT_LIST_DIR}/test_win_mgr.c")
	add_executable(test_server "${CMAKE_CURRENT_LIST_DIR}/test_win_server.c")
	add_executable(test_client "${CMAKE_CURRENT_LIST_DIR}/test_win_client.c")
	
	target_link_libraries(test_mgr PRIVATE testrig)
	target_link_libraries(test_server PRIVATE testrig)
	target_link_libraries(test_client PRIVATE testrig)

	add_test(NAME discrete COMMAND test_mgr $<TARGET_FILE:test_server> $<TARGET_FILE:test_client>)
else()
	add_executable(test_fork "${CMAKE_CURRENT_LIST_DIR}/test_fork_ipc.c")
	add_executable(test_preset_path "${CMAKE_CURRENT_LIST_DIR}/test_preset_sockpath.c")

	target_link_libraries(test_fork PRIVATE testrig)
	target_link_libraries(test_preset_path PRIVATE testrig)

	add_test(NAME forking COMMAND test_fork)
	add_test(NAME preset_path COMMAND test_preset_path)
endif()


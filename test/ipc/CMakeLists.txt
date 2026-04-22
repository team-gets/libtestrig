function(make_ipc_test OUT_TGT_NAME IN_SOURCES)
    add_executable(${OUT_TGT_NAME} ${IN_SOURCES})
    target_link_libraries(${OUT_TGT_NAME} PRIVATE libtestrig)
    target_copy_dll(${OUT_TGT_NAME})
    add_test(NAME "${OUT_TGT_NAME}_test" COMMAND $<TARGET_FILE:${OUT_TGT_NAME}>)
endfunction(make_ipc_test OUT_TGT_NAME IN_SOURCES)

make_ipc_test(sock_test_suite "${CMAKE_CURRENT_LIST_DIR}/sock_naming.c")
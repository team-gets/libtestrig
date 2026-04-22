function(make_ipc_test OUT_TGT_NAME IN_SOURCES)

    if (${ARGC} GREATER 2)
        set(_other_args ${ARGV2})
    else()
        set(_other_args)
    endif()

    add_executable(${OUT_TGT_NAME} ${IN_SOURCES})
    target_link_libraries(${OUT_TGT_NAME} PRIVATE libtestrig)
    target_copy_dll(${OUT_TGT_NAME})
    target_compile_warn_all(${OUT_TGT_NAME})
    add_test(NAME "${OUT_TGT_NAME}_test" COMMAND $<TARGET_FILE:${OUT_TGT_NAME}> ${_other_args})

endfunction(make_ipc_test OUT_TGT_NAME IN_SOURCES)

make_ipc_test(sock_naming "${CMAKE_CURRENT_LIST_DIR}/sock_naming.c")
make_ipc_test(sock_communicating "${CMAKE_CURRENT_LIST_DIR}/sock_communicating.c" "parent")
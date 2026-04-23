make_unit_test(sock_naming "${CMAKE_CURRENT_LIST_DIR}/sock_naming.c")
make_unit_test(sock_communicating "${CMAKE_CURRENT_LIST_DIR}/sock_communicating.c" "parent")
make_unit_test(sock_quickopening "${CMAKE_CURRENT_LIST_DIR}/sock_quickopening.c")
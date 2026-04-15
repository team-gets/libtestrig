set(TESTRIG_SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/main.c"
	"${CMAKE_CURRENT_LIST_DIR}/args.c"
	"${CMAKE_CURRENT_LIST_DIR}/actions.c"
	"${CMAKE_CURRENT_LIST_DIR}/help.c"
	"${CMAKE_CURRENT_LIST_DIR}/daemon.c")

add_executable(testrig ${TESTRIG_SOURCES})
target_link_libraries(testrig PRIVATE libtestrig)

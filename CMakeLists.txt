cmake_minimum_required(VERSION 3.30..4.0)
project(libtestrig
	LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(target_copy_dll)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(IPC_DIR "${LIB_DIR}/ipc")
set(EPOS_DIR "${LIB_DIR}/epos2")

set(IPC_SOURCES
	"${IPC_DIR}/ipc.c"
	"${IPC_DIR}/constants.c"
	"${IPC_DIR}/message.c"
	"${IPC_DIR}/os.c")

set(EPOS_SOURCES
	"${EPOS_DIR}/epos2.c"
	"${EPOS_DIR}/connect.c"
	"${EPOS_DIR}/definitions.c"
	"${EPOS_DIR}/manage.c"
	"${EPOS_DIR}/identify.c")

set(LIBTESTRIG_SOURCES ${IPC_SOURCES})

find_package(EPOSCmd)
if (EPOSCmd_FOUND)
	list(APPEND LIBTESTRIG_SOURCES ${EPOS_SOURCES})
else()
	message("Controller capabilities not being built.")
endif()

add_library(testrig SHARED ${LIBTESTRIG_SOURCES})
target_compile_definitions(testrig PRIVATE COMPILING_TESTRIG_DLL)
target_include_directories(testrig PUBLIC ${LIB_DIR})

if (WIN32)
	target_link_libraries(testrig PRIVATE Ws2_32)
endif()

if (EPOSCmd_FOUND)
	target_link_libraries(testrig PUBLIC EPOSCmd)
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/test")

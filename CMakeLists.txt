cmake_minimum_required(VERSION 3.30..4.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(libtestrig
	LANGUAGES C)

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

set(LIBTESTRIG_SOURCES ${IPC_SOURCES} ${EPOS_SOURCES})

find_package(EposCmd REQUIRED)
if (EposCmd-FOUND)
	message("-- Found EPOS Command Library")
endif()

add_library(libtestrig SHARED ${LIBTESTRIG_SOURCES})
target_compile_definitions(libtestrig PRIVATE COMPILING_TESTRIG_DLL)
target_include_directories(libtestrig PUBLIC ${LIB_DIR})

target_link_libraries(libtestrig PUBLIC EposCmd)

if (WIN32)
	target_link_libraries(libtestrig PRIVATE Ws2_32)
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/src")

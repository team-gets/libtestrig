cmake_minimum_required(VERSION 3.30..4.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(libtestrig
	LANGUAGES C)

include(target_copy_dll)
include(target_output_to_bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(IPC_DIR "${LIB_DIR}/ipc")
set(EPOS_DIR "${LIB_DIR}/epos2")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")

set(IPC_SOURCES
	"${IPC_DIR}/ipc.c"
	"${IPC_DIR}/message.c"
	"${IPC_DIR}/os.c")

set(EPOS_SOURCES
	"${EPOS_DIR}/epos2.c"
	"${EPOS_DIR}/connect.c"
	"${EPOS_DIR}/definitions.c"
	"${EPOS_DIR}/manage.c"
	"${EPOS_DIR}/identify.c")

set(LIBTESTRIG_SOURCES ${IPC_SOURCES} ${EPOS_SOURCES})

if (MSVC)
	add_compile_options("/W4")
else()
	add_compile_options("-Wall")
endif()


if (WIN32)
	add_library(EposCmd SHARED IMPORTED)
	set_target_properties(EposCmd PROPERTIES
		IMPORTED_LOCATION "${BIN_DIR}/EposCmd64.dll"
		IMPORTED_IMPLIB "${BIN_DIR}/EposCmd64.lib")

	set(EposCmd-FOUND TRUE)
else()
	add_library(EposCmd SHARED IMPORTED)
	set_target_properties(EposCmd PROPERTIES
		IMPORTED_LOCATION "${BIN_DIR}/libEposCmd.so"
		IMPORTED_IMPLIB "")

	set(EposCmd-FOUND TRUE)
endif()

add_library(libtestrig SHARED ${LIBTESTRIG_SOURCES})
target_compile_definitions(libtestrig PRIVATE COMPILING_TESTRIG_DLL)
target_include_directories(libtestrig PUBLIC ${LIB_DIR})
target_link_libraries(libtestrig PUBLIC EposCmd)
target_output_to_bin(libtestrig)

if (WIN32)
	target_link_libraries(libtestrig PUBLIC Ws2_32 Kernel32)
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/src")

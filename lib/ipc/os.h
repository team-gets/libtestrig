#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef _WIN32
#include "win_headers.h"
#endif

#include "libtestrig_api.h"

#ifndef _WIN32
#define INVALID_SOCKET -1
#endif

TESTRIG_API int vscl_get_sock_destination(char* dest);
TESTRIG_API int vscl_make_new_proc(const char* prog, const char* args);

#ifdef _WIN32
TESTRIG_API void vscl_winprint_error(const TCHAR* msg);
#endif

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

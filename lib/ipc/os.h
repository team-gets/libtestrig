#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef _WIN32
#include "win_headers.h"
#endif

#include "libtestrig_api.h"
#include <stdint.h>

#ifdef _WIN32
#else
#define INVALID_SOCKET -1
#endif

TESTRIG_API int vscl_get_sock_destination(char* dest);
TESTRIG_API int vscl_make_new_proc(const char* prog, const char* args);
TESTRIG_API void vscl_sleep(uint32_t s);
TESTRIG_API void vscl_os_perror(const char* msg);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

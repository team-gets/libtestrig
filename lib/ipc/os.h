#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "libtestrig_api.h"

#ifndef _WIN32
#define INVALID_SOCKET -1
#endif

TESTRIG_API int vscl_get_sock_destination(char* dest);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

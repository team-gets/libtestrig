#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "libtestrig_api.h"

typedef struct {
#ifdef _WIN32
	HANDLE child_write;
	HANDLE child_read;
#else
	int linux_fds[2];
#endif
} vscl_pipe_t;

TESTRIG_API vscl_pipe_t vscl_create_pipes(void);
TESTRIG_API int vscl_read_pipe(vscl_pipe_t* pipe, vscl_byte_t* buf);
TESTRIG_API int vscl_write_pipe(vscl_pipe_t* pipe, vscl_byte_t* buf);

#ifdef __cplusplus
} // extern "C"
#endif

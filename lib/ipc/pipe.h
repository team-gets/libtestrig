#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include "ipc/win_headers.h"
#else
#include <unistd.h>
#endif

#include "libtestrig_api.h"

typedef struct {
#ifdef _WIN32
	HANDLE child_read;
	HANDLE child_write;
#else
	int read_fd;
	int write_fd;
#endif
} vscl_pipe_t;

TESTRIG_API vscl_pipe_t vscl_create_pipes(void);
TESTRIG_API int vscl_read_pipe(vscl_pipe_t* pipe, vscl_byte_t buf[], size_t nbytes);
TESTRIG_API int vscl_write_pipe(vscl_pipe_t* pipe, vscl_byte_t buf[], size_t nbytes);

#ifdef __cplusplus
} // extern "C"
#endif

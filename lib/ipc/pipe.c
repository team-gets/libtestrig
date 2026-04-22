#include <stdio.h>
#include "ipc/pipe.h"

vscl_pipe_t vscl_create_pipes() {

	vscl_pipe_t piper = { 0 };

#if _WIN32
	SECURITY_ATTRIBUTES sattrs = { 0 };
	sattrs.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sattrs.bInheritHandle = TRUE; 
	sattrs.lpSecurityDescriptor = NULL; 

	if (!CreatePipe(&piper.child_read, &piper.child_write, &sattrs, 0))
		vscl_winprint_error(L"While creating child read pipe");

	if (!SetHandleInformation(piper.child_read, HANDLE_FLAG_INHERIT, 0))
		vscl_winprint_error(L"While preventing child from taking parent pipe attributes");
		
#else
	int pipestat = pipe(piper.linux_fds);
	if (pipestat == -1) { perror("While creating pipes"); }

#endif

	return piper;
}

int vscl_read_pipe(vscl_pipe_t* pipe, vscl_byte_t* buf) {
	return 0;
}

int vscl_write_pipe(vscl_pipe_t* pipe, vscl_byte_t* buf) {
	return 0;
}

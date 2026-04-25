#include <stdio.h>
#include "ipc/pipe.h"
#include "ipc/os.h"

vscl_pipe_t vscl_create_pipes() {

	vscl_pipe_t piper = { 0 };

#if _WIN32
	SECURITY_ATTRIBUTES sattrs = { 0 };
	sattrs.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sattrs.bInheritHandle = TRUE; 
	sattrs.lpSecurityDescriptor = NULL; 

	if (!CreatePipe(&piper.child_read, &piper.child_write, &sattrs, 0))
		vscl_os_perror("While creating child read pipe");

	if (!SetHandleInformation(piper.child_read, HANDLE_FLAG_INHERIT, 0))
		vscl_os_perror("While preventing child from taking parent pipe attributes");
#else
	int pipefd[2] = { 0 };
	int pipestat = pipe(pipefd);
	if (pipestat == -1) { vscl_os_perror("While creating pipes"); }

	piper.read_fd = pipefd[0];
	piper.write_fd = pipefd[1];

#endif
	return piper;
}

int vscl_read_pipe(vscl_pipe_t* pipe, vscl_byte_t buf[], size_t nbytes) {
#if _WIN32
	return 0;
#else
	size_t read_actual = read(pipe->read_fd, buf, nbytes);
	return (read_actual == nbytes) ? 0 : -1;
#endif
}

int vscl_write_pipe(vscl_pipe_t* pipe, vscl_byte_t buf[], size_t nbytes) {
#if _WIN32
	return 0;
#else
	size_t write_actual = write(pipe->write_fd, buf, nbytes);
	return (write_actual == nbytes) ? 0 : -1;
#endif
}

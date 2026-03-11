#include <stdio.h>
#include <assert.h>

#include "ipc/ipc.h"
#include "ipc/message.h"
#include "ipc/constants.h"

const char* SOCKNAME = "/tmp/this_is_my_socket.sock";
#ifdef _WIN32
typedef int pid_t;
#endif

int main(int argc, char** argv) {
	struct sockaddr_un parent_sockaddr;
	struct sockaddr_un child_sockaddr;
	pid_t pid;
	int retstat;
	int parent;
	int fd;
	int sent;
	uint8_t buf[4096] = { 0 };

	struct RigMessage msg;
	uint8_t data[8] = { 'H', 'E', 'L', 'L', 'O', '!', '!', '\n' };
	SetMessage(&msg, HEAD_DC, data);

	parent_sockaddr.sun_family = AF_UNIX;
	strncpy(parent_sockaddr.sun_path, SOCKNAME, strnlen(SOCKNAME, 108));

	parent = SockSetup(&parent_sockaddr);
	if (parent == -1) { perror("parent socket failed to spawn"); return 1; }

	assert(strncmp(parent_sockaddr.sun_path, SOCKNAME, strnlen(SOCKNAME, 108)) == 0);

	retstat = SockBind(parent, &parent_sockaddr);
	if (retstat == -1) { perror("parent binding failure"); return 1; }

#ifndef _WIN32
	pid = fork();
	switch (pid) {
	case -1:
		perror("fork");
		return 1;
	case 0: // child
		fd = SockSetup(&child_sockaddr);
		if (fd == -1) { perror("child socket failed to spawn"); return 1; }

		retstat = SockConnect(fd, &parent_sockaddr);
		if (retstat == -1) { perror("child connection failure"); return 1; }

		sent = SockSend(fd, &msg);
		printf("Sent %i bytes\n", sent);

		retstat = SockClose(parent, &child_sockaddr);
		if (retstat == -1) { perror("child socket close error"); return 1; }

		return 0;
		break;
	default: // original parent
		retstat = SockListen(parent, 2);
		if (retstat == -1) { perror("listen error"); return 1; }

		SockReadOut(parent, &parent_sockaddr, buf, 4096, DC_WITH_CLIENT);
		printf("Parent will exit...\n");
		break;
	}
#endif

	retstat = SockClose(parent, &parent_sockaddr);
	if (retstat == -1) { perror("parent socket close error"); return 1; }
	printf("Parent received %s\n", buf);

	assert(memcmp(buf, "HELLO!!\n", 8) == 0);

	return 0;
}

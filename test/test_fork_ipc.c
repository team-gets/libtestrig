#include <stdio.h>
#include <assert.h>

#include "ipc/ipc.h"
#include "ipc/message.h"
#include "ipc/constants.h"

int main(int argc, char** argv) {
	struct sockaddr_un parent_sockaddr;
	struct sockaddr_un child_sockaddr;
	pid_t pid;
	int retstat;
	int parent;
	int fd;
	int sent;
	struct RigMessage msg;
	uint8_t bad_head[4] = { 'H', 'E', 'L', 'L' };
	uint8_t bad_msg[8] = { 'O', ' ', 'W', 'O', 'R', 'L', 'D', '!' };
	uint8_t buf[4096] = { 0 };
	SetMessage(&msg, bad_head, bad_msg);
	
	struct RigMessage msg1;
	SetMessage(&msg1, HEAD_STAY, bad_msg);

	struct RigMessage msg2;
	SetMessage(&msg2, HEAD_DC, bad_msg);

	parent = SockSetup(&parent_sockaddr);
	if (parent == -1) { perror("parent socket failed to spawn"); return 1; }

	retstat = SockBind(parent, &parent_sockaddr);
	if (retstat == -1) { perror("parent binding failure"); return 1; }

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

		printf("Will send multiple messages...\n");
		sent = SockSend(fd, &msg);
		printf("Sent %i bytes the first time\n", sent);
		sent = SockSend(fd, &msg1);
		printf("Sent %i bytes the second time\n", sent);
		sent = SockSend(fd, &msg2);
		printf("Sent %i bytes the third time\n", sent);

		retstat = SockClose(parent, &child_sockaddr);
		if (retstat == -1) { perror("child socket ret error"); return 1; }

		return 0;
		break;
	default: // original parent
		retstat = SockListen(parent, 2);
		if (retstat == -1) { perror("listen error"); return 1; }

		SockReadOut(parent, &parent_sockaddr, buf, 4096, DC_WITH_CLIENT);
		printf("Parent will exit...\n");
		break;
	}

	retstat = SockClose(parent, &parent_sockaddr);
	if (retstat == -1) { perror("parent socket ret error"); return 1; }
	printf("Parent received %s\n", buf);

	assert(memcmp(buf, "O WORLD!O WORLD!", 16) == 0);

	return 0;
}

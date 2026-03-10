#include <stdio.h>

#include "ipc/ipc.h"
#include "ipc/message.h"
#include "ipc/constants.h"

int main(int argc, char** argv) {
	struct sockaddr_un parent_sockaddr;
	struct sockaddr_un child_sockaddr;
	pid_t pid;
	int bindstat; int connstat; int listenstat; int closestat;
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

	bindstat = bind(parent, (struct sockaddr*)&parent_sockaddr, sizeof(parent_sockaddr));
	if (bindstat == -1) { perror("parent binding failure"); return 1; }

	pid = fork();
	switch (pid) {
	case -1:
		perror("fork");
		return 1;
	case 0: // child
		fd = SockSetup(&child_sockaddr);
		if (fd == -1) { perror("child socket failed to spawn"); return 1; }

		connstat = SockConnect(fd, &parent_sockaddr);
		if (connstat == -1) { perror("child connection failure"); return 1; }

		printf("Will send multiple messages...\n");
		sent = SockSend(fd, &msg);
		printf("Sent %i bytes the first time\n", sent);
		sent = SockSend(fd, &msg1);
		printf("Sent %i bytes the second time\n", sent);
		sent = SockSend(fd, &msg2);
		printf("Sent %i bytes the third time\n", sent);

		closestat = SockClose(parent, &child_sockaddr);
		if (closestat == -1) { perror("child socket close error"); return 1; }

		return 0;
		break;
	default: // original parent
		listenstat = listen(parent, 2);
		if (listenstat == -1) { perror("listen error"); return 1; }

		SockReadOut(parent, &parent_sockaddr, buf, DC_WITH_CLIENT);
		printf("Parent will exit...\n");
		break;
	}

	closestat = SockClose(parent, &parent_sockaddr);
	if (closestat == -1) { perror("parent socket close error"); return 1; }
	printf("Parent received %s\n", buf);

	return 0;
}

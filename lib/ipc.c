#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ipc.h"

int SockSetup(struct sockaddr_un* sockaddr_mut) {
	int fd;
	char sockpath[32] = { 0 };

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) { perror("Failed to create socket"); return -1; }

	srand(time(NULL));
	for (int i = 0; i < 32; i++) {
		sockpath[i] = rand() % (122 + 1 - 48) + 48;
	}

	strncpy(sockaddr_mut->sun_path, sockpath, 32 + 1);

	return fd;
}

int SockBind(const int fd, const struct sockaddr_un* sockaddr) {
	int bindstat;
	socklen_t socklen;

	socklen = sizeof(*sockaddr);
	bindstat = bind(fd, (struct sockaddr*)sockaddr, socklen);
	if (bindstat == -1) { perror("Failed to bind socket"); }

	return bindstat;
}

int SockConnect(const int fd, const struct sockaddr_un* sockaddr) {
	int connstat;
	socklen_t socklen;

	socklen = sizeof(*sockaddr);
	connstat = connect(fd, (struct sockaddr*)sockaddr, socklen);
	if (connstat == -1) { perror("Failed to connect to socket"); }

	return connstat;
}

int SockClose(const int fd) {
	int closestat;

	closestat = close(fd);

	return closestat;
}

int SockLoopReceive(const int fd) {
	int recvstat;

	while (1) {
		uint8_t buf[12];
		recvstat = recv(fd, buf, sizeof(buf), MSG_WAITALL);

		if (recvstat != -1) { printf("Received this: %s\n", buf); }
	}

	return 0;
}

int SockSend(const int fd, struct RigMessage* msg) {
	int nbytes;
	uint8_t buf[12];

	nbytes = send(fd, buf, sizeof(buf), MSG_NOSIGNAL);
	if (nbytes == -1) { perror("Clientside socket send error"); }

	return nbytes;
}



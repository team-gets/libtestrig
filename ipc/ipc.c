#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ipc.h"

int SockSetup(struct sockaddr_un* sockaddr_mut) {
	int fd;
	char sockpath[32 + 5] = { '/', 't', 'm', 'p', '/' };

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) { perror("Failed to create socket"); return -1; }

	srand(time(NULL));
	for (int i = 5; i < 32 + 5; i++) {
		int start = (i % 2 == 0) ? 'a' : 'A';
		sockpath[i] = rand() % (26 + 1) + start;
	}

	memset(sockaddr_mut, 0, sizeof(*sockaddr_mut));
	sockaddr_mut->sun_family = AF_UNIX;
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

int SockLoopReceive(const int fd, struct sockaddr_un* sockaddr) {
	int recvstat;
	int acceptstat;
	socklen_t socklen = sizeof(*sockaddr);

	while (1) {
		acceptstat = accept(fd, (struct sockaddr*)sockaddr, &socklen);
		if (acceptstat == -1) { continue; }
	
		uint8_t buf[12] = { 0 };
		recvstat = read(fd, buf, sizeof(buf));

		if (recvstat != -1) { printf("Received this: %s\n", buf); }
	}

	return 0;
}

int SockSend(const int fd, struct RigMessage* msg) {
	int nbytes;
	uint8_t buf[12] = { 0 };

	for (int i = 0; i < 8; i++) {
		buf[i] = msg->head[i];
	}

	for (int i = 4; i < 12; i++) {
		buf[i] = msg->data[i];
	}

	nbytes = send(fd, buf, sizeof(buf), MSG_NOSIGNAL);
	if (nbytes == -1) { perror("Clientside socket send error"); }

	return nbytes;
}



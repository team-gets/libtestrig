#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "constants.h"
#include "ipc.h"

int SockSetup(struct sockaddr_un* sockaddr_mut) {
	int fd;
	char sockpath[5 + 32 + 5 + 1] = { 0 };

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) { perror("Failed to create socket"); return -1; }

	strncpy(sockpath, "/tmp/", 6);
	srand(time(NULL));
	for (int i = 5; i < 32 + 5; i++) {
		int start = (i % 2 == 0) ? 'a' : 'A';
		sockpath[i] = rand() % (26 + 1) + start;
	}

	strncat(sockpath, ".sock", 6);

	memset(sockaddr_mut, 0, sizeof(*sockaddr_mut));
	sockaddr_mut->sun_family = AF_UNIX;
	strncpy(sockaddr_mut->sun_path, sockpath, 5 + 32 + 5 + 1);

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

int SockClose(const int fd, struct sockaddr_un* sockaddr) {
	int closestat;

	closestat = close(fd);
	remove(sockaddr->sun_path);

	return closestat;
}

int SockReadOut(const int fd, const struct sockaddr_un* sockaddr, uint8_t* buf_out, size_t max_write, int flags) {
	int recvstat;
	int acceptstat;
	int head;

	int bytes_written = 0;
	int recent_success = 1;
	int reading = 1;
	socklen_t socklen = sizeof(*sockaddr);

	while (reading != -1) {
		acceptstat = accept(fd, (struct sockaddr*)sockaddr, &socklen);
		if (acceptstat == -1) { continue; }
	
		// While a child is connected to this socket...
		while (reading != -1) {
			uint8_t buf[12] = { 0 };
			recvstat = read(acceptstat, buf, sizeof(buf));

			if (recvstat == -1) { perror("Socket read failure"); continue; }

			uint8_t headcheck[4] = { buf[0], buf[1], buf[2], buf[3] };
			head = IdentifyFullHeader(headcheck);

			// Pick what to do
			switch (head) {
			case HEADER_IS_DC: // Disconnecting
				if (flags & DC_WITH_CLIENT)
					reading = -1;

				break;
			case HEADER_IS_STAY: // Continue
				recent_success = 1;
				break;
			case -1: // Invalid header: don't read
				if (recent_success == 1) {
					printf("Socket receive error: Invalid header\n");
					recent_success = 0;
				}

				continue;
				break;
			default: // Not really possible, but don't read it regardless.
				if (recent_success == 1) {
					printf("Unknown header read error\n");
					recent_success = 0;
				}

				continue;
				break;
			}

			// Do a readout
			for (int i = 4; i < 12 && bytes_written < 4096; i++, bytes_written++) {
				buf_out[bytes_written] = buf[i];
			}
		}
	}

	return 0;
}

int SockReadAndHandle(const int fd, struct sockaddr_un* sockaddr, int(*handler)(uint8_t*)) {
	int recvstat;
	int acceptstat;
	int handlestat;

	int reading = 1;
	socklen_t socklen = sizeof(*sockaddr);

	while (reading != -1) {
		acceptstat = accept(fd, (struct sockaddr*)sockaddr, &socklen);
		if (acceptstat == -1) { continue; }
	
		// While a child is connected to this socket...
		while (reading != -1) {
			uint8_t buf[12] = { 0 };
			recvstat = read(acceptstat, buf, sizeof(buf));

			if (recvstat == -1) { perror("Socket read failure"); continue; }

			handlestat = handler(buf);

			if (handlestat & HANDLER_RET_DC) {
				reading = -1;
			}
		}
	}

	return 0;
}

int SockSend(const int fd, struct RigMessage* msg) {
	int nbytes;
	uint8_t buf[12] = { 0 };

	for (int i = 0; i < 4; i++) {
		buf[i] = msg->head[i];
	}

	for (int i = 0; i < 8; i++) {
		buf[i + 4] = msg->data[i];
	}

	nbytes = write(fd, buf, 12);
	if (nbytes == -1) { perror("Clientside socket send error"); }

	return nbytes;
}

int IdentifyHeaderPart(uint8_t in[4], int idx) {
	if (in[idx] == HEAD_STAY[idx]) {
		return HEADER_IS_STAY;
	}
	else if (in[idx] == HEAD_DC[idx]) {
		return HEADER_IS_DC;
	}
	else {
		return -1;
	}
}

int IdentifyFullHeader(uint8_t in[4]) {
	int identity = -1;

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			identity = IdentifyHeaderPart(in, 0);
		}
		else {
			identity &= IdentifyHeaderPart(in, i);
		}
	}
	printf("Ident %i\n", identity);

	return identity;
}

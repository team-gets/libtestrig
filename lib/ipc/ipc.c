#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ipc.h"
#include "constants.h"
#include "os.h"

#ifdef _WIN32
// TODO: Evaluate if this is sufficient (it is honestly kind of smelly)
typedef int socklen_t;
#endif

int vscl_sock_generate_path(char* sockpath) {
	int retstat;
	int baselen;
	//int dstart;

	retstat = vscl_get_sock_destination(sockpath);
	baselen = strlen(sockpath);

	if (retstat != 0) { return retstat; }

	srand(time(NULL));
	for (int i = baselen; i < 32 + baselen - 1; i++) {
		int start = (i % 2 == 0) ? 'a' : 'A';
		sockpath[i] = rand() % (25 + 1) + start;
	}

	strncat(sockpath, ".rigsock", 9);
	return 0;
}

int vscl_sock_setup(struct sockaddr_un* sockaddr_mut) {
	int fd;
	int path_set;
	char sockpath[108] = { 0 };
	char blank[108] = { 0 };

#ifdef _WIN32
	WSADATA wsa_data;
	int wsa_result;

	wsa_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (wsa_result != 0) { perror("Failed WSAStartup"); return -1; }
#endif // _WIN32

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == INVALID_SOCKET) { perror("Failed to create socket"); return -1; }

	path_set = (strncmp(
		sockaddr_mut->sun_path, blank,
		strnlen(sockaddr_mut->sun_path, 108)) == 0) ? -1 : 1;

	if (path_set == -1) {
		vscl_sock_generate_path(sockpath);
		long long int sockpathlen = strnlen(sockpath, 108);
		strncpy(sockaddr_mut->sun_path, sockpath, sockpathlen + 1);
	}

	sockaddr_mut->sun_family = AF_UNIX;
	return fd;
}

int vscl_sock_bind(const int fd, const struct sockaddr_un* sockaddr) {
	int bindstat;
	socklen_t socklen;

	socklen = sizeof(*sockaddr);
	bindstat = bind(fd, (struct sockaddr*)sockaddr, socklen);
	if (bindstat == -1) { perror("Failed to bind socket"); }

	return bindstat;
}

int vscl_sock_listen(const int fd, int max_backlog) {
	int listenstat;

	listenstat = listen(fd, max_backlog);
	if (listenstat == -1) { perror("Failed to set socket to listen"); }

	return listenstat;
}

int vscl_sock_connect(const int fd, const struct sockaddr_un* sockaddr) {
	int connstat;
	socklen_t socklen;

	socklen = sizeof(*sockaddr);
	connstat = connect(fd, (struct sockaddr*)sockaddr, socklen);
	if (connstat == -1) { perror("Failed to connect to socket"); }

	return connstat;
}

int vscl_sock_close(const int fd, struct sockaddr_un* sockaddr) {
	int closestat;


#ifdef _WIN32
	closestat = closesocket(fd);
#else
	closestat = close(fd);
#endif
	remove(sockaddr->sun_path);

	return closestat;
}

// TODO: reevaluate necessity for inclusion
int vscl_sock_read_out(const int fd, const struct sockaddr_un* sockaddr, uint8_t* buf_out, size_t max_write, int flags) {
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
#ifdef _WIN32
			recvstat = recv(acceptstat, buf, 12, MSG_PEEK);
#else
			recvstat = read(acceptstat, buf, 12);
#endif

			if (recvstat == -1) { perror("Socket read failure"); continue; }

			uint8_t headcheck[4] = { buf[0], buf[1], buf[2], buf[3] };
			head = vscl_identify_full_header(headcheck);

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

int vscl_sock_read_and_handle(const int fd, struct sockaddr_un* sockaddr, int(*handler)(uint8_t*)) {
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
#ifdef _WIN32
			recvstat = recv(acceptstat, buf, 12, MSG_PEEK);
#else
			recvstat = read(acceptstat, buf, 12);
#endif

			if (recvstat == -1) { perror("Socket read failure"); continue; }

			handlestat = handler(buf);

			if (handlestat == HANDLER_RET_DC) {
				reading = -1;
			}
		}
	}

	return 0;
}

int vscl_sock_send(const int fd, struct rig_message* msg) {
	int nbytes;
	uint8_t buf[12] = { 0 };

	for (int i = 0; i < 4; i++) {
		buf[i] = msg->head[i];
	}

	for (int i = 0; i < 8; i++) {
		buf[i + 4] = msg->data[i];
	}

#ifdef _WIN32
	nbytes = send(fd, buf, 12, MSG_DONTROUTE);
#else
	nbytes = write(fd, buf, 12);
#endif
	if (nbytes == -1) { perror("Clientside socket send error"); }

	return nbytes;
}

int vscl_identify_header_part(uint8_t in[4], int idx) {
	if (in[idx] == HEAD_STAY[idx]) {
		return HEADER_IS_STAY;
	}
	else if (in[idx] == HEAD_DC[idx]) {
		return HEADER_IS_DC;
	}
	else if (in[idx] == HEAD_SYNC[idx]) {
		return HEADER_IS_SYNC;
	}
	else {
		return -1;
	}
}

int vscl_identify_full_header(uint8_t in[4]) {
	int identity = -1;

	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			identity = vscl_identify_header_part(in, 0);
		}
		else {
			identity &= vscl_identify_header_part(in, i);
		}
	}

	return identity;
}

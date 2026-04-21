#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sock.h"
#include "os.h"

#ifdef _WIN32
// TODO: Evaluate if this is sufficient (it is honestly kind of smelly)
typedef int socklen_t;
#endif

int vscl_sock_genpath(char* sockpath) {
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
	if (wsa_result != 0) { perror("failed WSAStartup"); return -1; }
#endif // _WIN32

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == INVALID_SOCKET) { perror("failed to create socket"); return -1; }

	path_set = (strncmp(
		sockaddr_mut->sun_path, blank,
		strnlen(sockaddr_mut->sun_path, 108)) == 0) ? -1 : 1;

	if (path_set == -1) {
		vscl_sock_genpath(sockpath);
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
	if (bindstat == -1) { perror("failed to bind socket"); }

	return bindstat;
}

int vscl_sock_listen(const int fd, int max_backlog) {
	int listenstat;

	listenstat = listen(fd, max_backlog);
	if (listenstat == -1) { perror("failed to set socket to listen"); }

	return listenstat;
}

int vscl_sock_connect(const int fd, const struct sockaddr_un* sockaddr) {
	int connstat;
	socklen_t socklen;

	socklen = sizeof(*sockaddr);
	connstat = connect(fd, (struct sockaddr*)sockaddr, socklen);
	if (connstat == -1) { perror("failed to connect to socket"); }

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

int vscl_sock_send(const int fd, struct rig_message* msg) {
	int nbytes;
	vscl_byte_t buf[12] = { 0 };

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

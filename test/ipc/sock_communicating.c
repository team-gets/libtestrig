#include "ipc/sock.h"
#include "ipc/os.h"
#include <stdio.h>

static const char* TESTER_SOCK_NAME = "TEST_SOCK.rigsock"; // NOLINT

int main(int argc, char** argv) {
    printf("This was invoked with the following command line:\n");
    printf("\t%s %s\n", argv[0], argv[1]);
		
	char sockpath[108] = { 0 };
	int retstat = vscl_get_sock_destination(sockpath);
	if (retstat != 0) {
		fprintf(stderr,
				"While preparing the sockets, socket destination grabbing failed.\n");
		return retstat;
	}

	strncat(sockpath, TESTER_SOCK_NAME, 19);
	struct sockaddr_un sockaddr = { .sun_family = AF_UNIX, };
	strncpy(sockaddr.sun_path, sockpath, 108);

    if (strncmp(argv[1], "parent", 6) == 0) {
		int parented = vscl_sock_setup(&sockaddr);
		if (parented == INVALID_SOCKET) { fprintf(stderr, "Parent sock maker fail\n"); return -1; }

		int parentstat = vscl_sock_bind(parented, &sockaddr);
		if (parentstat == -1) { fprintf(stderr, "Sock binder fail\n"); return parentstat; }

		parentstat = vscl_sock_listen(parented, 1);
		if (parentstat == -1) {
			int closestat = vscl_sock_close(parented, &sockaddr);
			return (!closestat) ? closestat : parentstat;
		}

		// Race condition?
        parentstat = vscl_make_new_proc(argv[0], "child");
        if (parentstat == -1) { fprintf(stderr, "Failure to launch\n"); return parentstat; }
        else { printf("New process was spawned with PID %i\n", parentstat); }

		int exitplz = 0;
		socklen_t socklen = (socklen_t)sizeof(sockaddr);
        while (!exitplz) {
			int acceptor = accept(parented, (struct sockaddr*)&sockaddr, &socklen);
			if (acceptor == -1) { fprintf(stderr, "Accept epic fail\n"); continue; }

			vscl_byte_t buf[12] = { 0 };

#ifdef _WIN32
			int recvd = recv(accepted, buf, 12, MSG_PEEK);
#else
			int recvd = read(acceptor, buf, 12);
#endif
			if (recvd != 12) {
				fprintf(stderr, "Failure to read full msg: %i out of 12\n", recvd);
				vscl_sock_close(parented, &sockaddr);
				return recvd;
			}

			printf("Success: The message I got was %s\n", buf);
			assert(strncmp(buf, "HOWDY WORLD", 12) == 0);
		}

		return vscl_sock_close(parented, &sockaddr);
    }
	else if (strncmp(argv[1], "child", 6) == 0) {
		struct sockaddr_un childsock = { 0 };
		int childed = vscl_sock_setup(&childsock);
		if (childed == INVALID_SOCKET) { fprintf(stderr, "Child sock maker fail\n"); return -1; }
		
		int connection = vscl_sock_connect(childed, &sockaddr);
		if (connection == -1) { fprintf(stderr, "Child connection fail\n"); return connection; }

		struct rig_message msg = { {'H', 'O', 'W', 'D'}, {'Y', ' ', 'W', 'O', 'R', 'L', 'D', 0} };
		vscl_sock_send(connection, &msg);

		return vscl_sock_close(childed, &childsock);
	}
}

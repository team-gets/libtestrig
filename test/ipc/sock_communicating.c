#include "ipc/sock.h"
#include "ipc/os.h"
#include <stdio.h>

static const char* TESTER_SOCK_NAME = "TEST_SOCK.rigsock"; // NOLINT

int main(int argc, char** argv) {
	if (argc < 2) { fprintf(stderr, "You must pass arguments to this test.\n"); return -1; }

    printf("This was invoked with the following command line:\n");
    printf("\t%s %s\n", argv[0], argv[1]);
	fclose(stdin);
		
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

    if (strncmp(argv[1], "parent", 7) == 0) {
		int forkstat = vscl_make_new_proc(argv[0], "child");
		if (forkstat == -1) { fprintf(stderr, "Failure to launch\n"); return forkstat; }
		else { printf("New process was spawned with PID %i\n", forkstat); }

		int parented = vscl_sock_setup(&sockaddr);
		if (parented == INVALID_SOCKET) {
			fprintf(stderr, "Parent sock maker fail\n");
			return -1;
		}

		int parentstat = vscl_sock_bind(parented, &sockaddr);
		if (parentstat == -1) {
            fprintf(stderr, "Sock binder fail\n");
			int closestat = vscl_sock_close(parented, &sockaddr);
			return (!closestat) ? closestat : parentstat;
        }

		parentstat = vscl_sock_listen(parented, 1);
		if (parentstat == -1) {
            fprintf(stderr, "Sock listener fail at sockpath %s\n", sockaddr.sun_path);
			int closestat = vscl_sock_close(parented, &sockaddr);
			return (!closestat) ? closestat : parentstat;
		}

		int exitplz = 0;
		socklen_t socklen = (socklen_t)sizeof(struct sockaddr);
        while (!exitplz) {
			int acceptor = accept(parented, (struct sockaddr*)&sockaddr, &socklen);
			if (acceptor == -1) { fprintf(stderr, "Accept epic fail\n"); continue; }

			vscl_byte_t buf[12] = { 0 };

#ifdef _WIN32
			int recvd = recv(acceptor, buf, 12, MSG_PEEK);
#else
			int recvd = read(acceptor, buf, 12);
#endif
			if (recvd != 12) {
				fprintf(stderr, "Failure to read full msg: %i out of 12\n", recvd);
				vscl_sock_close(parented, &sockaddr);
				return (recvd == 0) ? -1 : recvd;
			}

			printf("Success: The message I got was %s\n", buf);
			assert(strncmp(buf, "HOWDY WORLD", 12) == 0);

			exitplz = 1;
		}

		return vscl_sock_close(parented, &sockaddr);
    }
	else if (strncmp(argv[1], "child", 6) == 0) {
		vscl_sleep(5); // arbitrary

		struct sockaddr_un childsock = { 0 };
		int childed = vscl_sock_setup(&childsock);
		if (childed == INVALID_SOCKET) { fprintf(stderr, "Child sock maker fail\n"); return -1; }
		else { printf("Child socket created at %s\n", childsock.sun_path); }
		
		int connection = vscl_sock_connect(childed, &sockaddr);
		if (connection == -1) {
            fprintf(stderr, "Child connection fail\n");
            vscl_sock_close(childed, &childsock);
			int closestat = vscl_sock_close(childed, &sockaddr);
			return (!closestat) ? closestat : connection;
        }

		struct rig_message msg = { {'H', 'O', 'W', 'D'}, {'Y', ' ', 'W', 'O', 'R', 'L', 'D', 0} };
		int sent = vscl_sock_send(connection, &msg);
		if (sent != 12) {
			fprintf(stderr, "Failure to send all bytes: %i out of 12\n", sent);
            vscl_sock_close(childed, &childsock);
			int closestat = vscl_sock_close(childed, &sockaddr);
			return (!closestat) ? closestat : -1;
		}
		else {
			printf("Sent all bytes.\n");
		}

		return vscl_sock_close(childed, &childsock);
	}
	else {
		fprintf(stderr, "Invalid token: %s\n", argv[1]);
		return -1;
	}
}

#define _XOPEN_SOURCE_EXTENDED 1
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <string.h>
#include "daemon.h"
#include "ipc/os.h"

#ifdef _WIN32
#include <winsock2.h>
//#include <windows.h> // holy cow there is some include order stuff with windows.h
typedef int socklen_t;
#else
#include <ftw.h>
#include <unistd.h>
#include <signal.h>
#endif // _WIN32

extern enum TESTRIG_DAEMON_STATE DAEMON_CURRENT_STATUS;

#ifdef _WIN32
#else
static void interrupt_catcher(int sig, siginfo_t* info, void* ucontext) {
	if (sig != SIGINT || info->si_signo != SIGINT) { return; }
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_STOPPED;
}

static int impl_look_for_sock_ext(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf) {
	return 0;
}
#endif // _WIN32

static int seek_sock_ext(char* sock) {
#ifdef _WIN32
#else
	char* orig = { 0 };
	strncpy(orig, sock, 108);

	int walker = nftw(sock, impl_look_for_sock_ext, 10, FTW_MOUNT | FTW_PHYS);
#endif
}

int seek_daemon(struct sockaddr_un* sockaddr) {
	char sock[108] = { 0 };
	int dest = GetSockDestination(sock);
	if (dest) { return 1; }

	sockaddr->sun_family = AF_UNIX;
	strncpy(sockaddr->sun_path, sock, 108);

	return 0;
}

int testrig_daemon(other_args* others) {
	struct sockaddr_un sockaddr = { 0 };
	int retstat = 0;

	int sock = SockSetup(&sockaddr);
	if (sock == -1) { return -1; }
	socklen_t socksize = sizeof(sockaddr);

	retstat = SockBind(sock, &sockaddr);
	if (retstat == -1) { return -1; }

	retstat = SockListen(sock, 1);
	if (retstat == -1) { return -1; }

#ifdef _WIN32
#else
	struct sigaction act = { 0 };
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &interrupt_catcher;

	int sigint_bound = sigaction(SIGINT, &act, NULL);
	if (sigint_bound == -1) { perror("daemon signal capture"); return -1; }
#endif // _WIN32

	printf("Waiting for connection...\n");
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_LISTENING;
	while (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_LISTENING) {
		int accepted = accept(sock, (struct sockaddr*)&sockaddr, &socksize);
		if (accepted == -1) { perror("daemon accept failure"); continue; }

		uint8_t msg[12] = { 0 };
#ifdef _WIN32
		int synced = recv(accepted, msg, 12, MSG_PEEK);
#else
		int synced = read(accepted, msg, 12);
#endif
		if (synced != 12) { continue; }

		uint8_t head[4] = { 0 };
		memcpy(head, msg, 4);

		int header = IdentifyFullHeader(head);
		if (header != HEADER_IS_SYNC) { continue; }

		// What i want:
		// - It should read the msg
		// - It should check if it's a sync msg
		// - It should reply (i think i can do this with bytestreasm)
		// - It should connect back to another socket to send the data!
		struct RigMessage reply;
		uint8_t blank[8] = { 0 };
		int set = SetMessage(&reply, HEAD_SYNC, blank);
		if (!set) { continue; }

		int sent = SockSend(accepted, &reply);
		if (sent != 12) { continue; }



		DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_CONNECTED;
		printf("Daemon accepted connection...\n");

		while (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_CONNECTED) {
			// this isn't cfg right
			int accepted = SockConnect(sock, &sockaddr);
			if (accepted == -1) { perror("daemon connect failure"); continue; }

			uint8_t buf[12] = { 0 };
#ifdef _WIN32
			int recvd = recv(accepted, buf, 12, MSG_PEEK);
#else
			int recvd = read(accepted, buf, 12);
#endif
			if (recvd != 12) { continue; } // TODO: some contingency?

			// TODO: proper impl that pipes this stuff (into a socket or file or stdout)
			printf("The message... %s\n", buf);
			break;
		}
	}

	SockClose(sock, &sockaddr);
	printf("Stopping...\n");
	return 0;
}

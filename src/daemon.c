// NOLINTBEGIN
#define _XOPEN_SOURCE_EXTENDED 1
#define _XOPEN_SOURCE 600
// NOLINTEND

#include <stdio.h>
#include <string.h>
#include "daemon.h"
#include "ipc/ipc.h"

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h> // holy cow there is some include order stuff with windows.h
typedef int socklen_t;
#else
#include <ftw.h>
#include <unistd.h>
#include <signal.h>
#endif // _WIN32

extern enum TESTRIG_DAEMON_STATE DAEMON_CURRENT_STATUS; // NOLINT
static const char* SOCK_FNAME = "testrigd.sock"; // NOLINT

// Ctrl-C (Interrupt) Catchers/Handlers {{{
#ifdef _WIN32
static struct sockaddr_un* daemon_sockaddr;
static SOCKET daemon_sock;

BOOL WINAPI interrupt_catcher(DWORD ctrl_type) {
	if (ctrl_type == CTRL_C_EVENT) {
		DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_STOPPED;
		printf("Stopping!\n");
		vscl_sock_close(daemon_sock, daemon_sockaddr);
		return TRUE;
	}

	return FALSE;
}
#else
static void interrupt_catcher(int sig, siginfo_t* info, [[ maybe_unused ]] void* ucontext) {
	if (sig != SIGINT || info->si_signo != SIGINT) { return; }
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_STOPPED;
}
#endif // _WIN32: Clean Ctrl+C handlers

static int deploy_interrupt_cleanup([[maybe_unused]] int sock, [[maybe_unused]] struct sockaddr_un* sockaddr) {
#ifdef _WIN32
	BOOL setted = SetConsoleCtrlHandler(interrupt_catcher, TRUE);
	if (!setted) { vscl_winprint_error("daemon ctrl handler"); return 0; }
	
	daemon_sockaddr = sockaddr;
	daemon_sock = sock;
	return 1;
#else
	struct sigaction act = { 0 };
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &interrupt_catcher;

	int sigint_bound = sigaction(SIGINT, &act, NULL);
	if (sigint_bound == -1) { perror("daemon signal capture"); return 0; }
	return 1;
#endif // _WIN32: Setup signal handler
} // interrupt intercept maker
// }}} fold

// Daemon Process and Socket Identification {{{
int seek_daemon(struct sockaddr_un* sockaddr) {
	char sockpath[108] = { 0 };

	int sockcopied = vscl_get_sock_destination(sockpath);
	if (sockcopied != 0) { return 0; }
	strncat(sockpath, SOCK_FNAME, 15);
	strncpy(sockaddr->sun_path, sockpath, 108);

	FILE* sockf = fopen(sockpath, "r");
	if (sockf == NULL) { return 0; }

	fclose(sockf);
	return 1;
} // int seek_daemon(struct sockaddir_un* sockaddr)
// }}} fold

int testrig_daemon([[maybe_unused]] other_args* others) {
	struct sockaddr_un sockaddr = { .sun_family = AF_UNIX };
	int sought = seek_daemon(&sockaddr);
	if (sought) { fprintf(stderr, "error: daemon already running"); return -1; }

	int retstat = 0;

	int sock = vscl_sock_setup(&sockaddr);
	if (sock == -1) { return -1; }
	socklen_t socksize = sizeof(sockaddr);

	if (!deploy_interrupt_cleanup(sock, &sockaddr)) {
		vscl_sock_close(sock, &sockaddr);
		return -1;
	}

	retstat = vscl_sock_bind(sock, &sockaddr);
	if (retstat == -1) {
		vscl_sock_close(sock, &sockaddr);
		return -1;
	}

	retstat = vscl_sock_listen(sock, 1);
	if (retstat == -1) {
		vscl_sock_close(sock, &sockaddr);
		return -1;
	}

	printf("Waiting for connection...\n");
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_LISTENING;
	while (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_LISTENING) {
		int accepted = accept(sock, (struct sockaddr*)&sockaddr, &socksize);
		if (accepted == -1) { perror("daemon accept failure"); continue; }

		printf("Accepted\n");
		vscl_byte_t buf[12] = { 0 };

#ifdef _WIN32
		int recvd = recv(accepted, buf, 12, MSG_PEEK);
#else
		int recvd = read(accepted, buf, 12);
#endif
		if (recvd != 12) { perror("did not read full msg\n"); continue; }

		vscl_byte_t body[8] = { 0 };
		for (uint8_t i = 4; i < 12; i++) {
			body[i - 4] = buf[i];
		}

		if (!strncmp("STATUS", body, 7))			{ testrig_stat(NULL); }
		else if (!strncmp("OPEN", body, 5))			{ testrig_open(NULL); }
		else if (!strncmp("REQUEST", body, 7))		{ testrig_request(NULL); }
		else if (!strncmp("CLOSE", body, 6))		{ testrig_close(NULL); }
	}

	vscl_sock_close(sock, &sockaddr);
	printf("Stopping...\n");
	return 0;
} // int testrig_daemon(other_args* others)
// vim: foldmethod=marker

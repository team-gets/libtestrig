// NOLINTBEGIN
#define _XOPEN_SOURCE_EXTENDED 1
#define _XOPEN_SOURCE 600
// NOLINTEND

#include <stdio.h>
#include <string.h>
#include "daemon.h"
#include "ipc/ipc.h"

#ifdef _WIN32
#include "ipc/win_headers.h"
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
		DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_CLEANING;
		printf("Stopping!\n");
		vscl_sock_close(daemon_sock, daemon_sockaddr);
		return TRUE;
	}

	return FALSE;
}
#else
static void interrupt_catcher(int sig, siginfo_t* info, [[ maybe_unused ]] void* ucontext) {
	if (sig != SIGINT || info->si_signo != SIGINT) { return; }
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_CLEANING;
}
#endif // _WIN32: Clean Ctrl+C handlers

static int deploy_interrupt_cleanup([[maybe_unused]] int sock, [[maybe_unused]] struct sockaddr_un* sockaddr) {
#ifdef _WIN32
	BOOL setted = SetConsoleCtrlHandler(interrupt_catcher, TRUE);
	if (!setted) { vscl_os_perror("daemon ctrl handler"); return 0; }
	
	daemon_sockaddr = sockaddr;
	daemon_sock = sock;
	return 1;
#else
	struct sigaction act = { 0 };
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &interrupt_catcher;

	int sigint_bound = sigaction(SIGINT, &act, NULL);
	if (sigint_bound == -1) { vscl_os_perror("daemon signal capture"); return 0; }
	return 1;
#endif // _WIN32: Setup signal handler
} // interrupt intercept maker
// }}} fold

// Daemon Process and Socket Identification {{{
int seek_daemon(struct sockaddr_un* sockaddr) {
	char sockpath[108] = { 0 };

	int sockcopied = vscl_get_sock_destination(sockpath);
	if (sockcopied != 0) { return 0; }
	memset(sockaddr->sun_path, 0, 108);
	sprintf(sockaddr->sun_path, "%s%s", sockpath, SOCK_FNAME);

	struct sockaddr_un temp_sockaddr = { 0 };
	int temp_sock = vscl_sock_setup(&temp_sockaddr);
	if (temp_sock == INVALID_SOCKET) { return 0; }

	int connstat = connect(temp_sock, (struct sockaddr*)sockaddr, (socklen_t)sizeof(*sockaddr));
	if (connstat == -1) { return 0; }

#ifdef _WIN32
	closesocket(temp_sock);
#else
	close(temp_sock);
#endif

	return 1;
} // int seek_daemon(struct sockaddir_un* sockaddr)
// }}} Daemon Process and Socket Identification

int testrig_daemon(other_args* others) {
	int retstat = 0;
	struct sockaddr_un sockaddr = { .sun_family = AF_UNIX };

	int sock = vscl_sock_setup(&sockaddr);
	if (sock == -1) { return -1; }
	socklen_t socksize = sizeof(sockaddr);

	int sought = seek_daemon(&sockaddr);

	if (others != NULL && others->data != NULL
	    && !strncmp(others->data[0], "down", 6)) {
		
		vscl_byte_t body[8] = "DOWN";
		struct rig_message msg = { 0 };

		vscl_set_message(&msg, HEAD_SYNC, body);
		int conn = vscl_sock_connect(sock, &sockaddr);
		if (conn == -1) { return -1; }

		int sent = vscl_sock_send(sock, &msg);
		if (sent == -1) { return -1; }

		return 0;
	}
	else if (sought) {
		fprintf(stderr, "Error: Daemon already running\n");
		return -1;
	}

	if (!deploy_interrupt_cleanup(sock, &sockaddr)) {
		vscl_sock_close(sock, &sockaddr);
		return -1;
	}

	retstat = vscl_sock_bind(sock, &sockaddr);
	if (retstat == -1) {
		vscl_sock_close(sock, &sockaddr);
		return -1;
	}

	retstat = vscl_sock_listen(sock, 5);
	if (retstat == -1) {
		vscl_sock_close(sock, &sockaddr);
		return -1;
	}

	printf("Waiting for connection...\n");
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_LISTENING;
	while (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_LISTENING) {
		int accepted = accept(sock, (struct sockaddr*)&sockaddr, &socksize);
		if (accepted == -1) {
			if (DAEMON_CURRENT_STATUS != TESTRIG_DAEMON_CLEANING)
				vscl_os_perror("Daemon accept failure");

			continue;
		}

		vscl_byte_t buf[12] = { 0 };

#ifdef _WIN32
		int recvd = recv(accepted, buf, 12, MSG_PEEK);
#else
		int recvd = read(accepted, buf, 12);
#endif
		if (recvd != 12) {
			if (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_LISTENING
			||  DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_CONNECTED) {
				vscl_os_perror("Did not read full msg");
			}

			continue;
		}

		vscl_byte_t body[8] = { 0 };
		for (uint8_t i = 4; i < 12; i++) {
			body[i - 4] = buf[i];
		}

		if (!strncmp("STATUS", body, 7))		{ testrig_stat(NULL); }
		else if (!strncmp("OPEN", body, 5))		{ testrig_open(NULL); }
		else if (!strncmp("REQUEST", body, 7))	{ testrig_peek(NULL); }
		else if (!strncmp("CLOSE", body, 6))	{ testrig_close(NULL); }
		else if (!strncmp("DOWN", body, 5))		{ DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_CLEANING; }
	}

	vscl_sock_close(sock, &sockaddr);
	printf("Stopping...\n");
	return 0;
} // int testrig_daemon(other_args* others)
// vim: foldmethod=marker

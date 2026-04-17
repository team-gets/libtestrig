// NOLINTBEGIN
#define _XOPEN_SOURCE_EXTENDED 1
#define _XOPEN_SOURCE 600
// NOLINTEND

#include <stdio.h>
#include <string.h>
#include <pthread.h>
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
static const char* sockfname = "testrigd.sock";

pthread_t* connector = 0;
int connectorstat = 0;
typedef struct {
	int acceptfd;
	int sock;
	struct sockaddr_un* sockaddr;
} delegate_info_t;

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
#ifndef _WIN32
static int impl_look_for_sock_ext(const char* fpath,
		[[ maybe_unused ]] const struct stat* sb, [[ maybe_unused ]] int tflag, [[ maybe_unused ]] struct FTW* ftwbuf) {
	if (tflag == FTW_F) {
		if (strncmp(fpath, sockfname, strlen(sockfname) + 1) == 0) {
			return 1;
		};
	}

	return 0;
}
#endif

static int seek_sock(char* sock) {
#ifdef _WIN32
	// TODO: where directory walker (steal previous impl from tests)
#else
	char orig[108] = { 0 };
	strncpy(orig, sock, 108);

	int walker = nftw(sock, impl_look_for_sock_ext, 10, FTW_MOUNT | FTW_PHYS);
	if (walker != 1) { perror("finder fail"); return 1; }

	printf("The sock %s\n", sockfname);
	return 0;
#endif
} // static int seek_sock_ext(char* sock)

int seek_daemon(struct sockaddr_un* sockaddr) {
	char sock[108] = { 0 };
	int dest = vscl_get_sock_destination(sock);
	if (dest) { return 1; }

	sockaddr->sun_family = AF_UNIX;

	// FIXME: this hopes that we clean up after ourselves and that only one exists
	int not_sought = seek_sock(sock);
	if (not_sought) { return 1; }

	vscl_get_sock_destination(sockaddr->sun_path);
	strncat(sockaddr->sun_path, sockfname, 108);

	return 0;
} // int seek_daemon(struct sockaddir_un* sockaddr)
// }}} fold

// Synchronize {{{
void* daemon_synchronize(void* arg) {
	delegate_info_t* dinfo = (delegate_info_t*)(arg);

	int acceptfd = dinfo->acceptfd;
	int sock = dinfo->sock;
	struct sockaddr_un* sockaddr = dinfo->sockaddr;

	vscl_byte_t msg[12] = { 0 };
#ifdef _WIN32
	int synced = recv(acceptfd, msg, 12, MSG_PEEK);
#else
	int synced = read(acceptfd, msg, 12);
#endif
	if (synced != 12) { return 0; }

	vscl_byte_t head[4] = { 0 };
	memcpy(head, msg, 4);

	int header = vscl_ident_full_header(head);
	if (header != HEADER_IS_SYNC) { return 0; }

	struct rig_message reply;
	vscl_byte_t blank[8] = { 0 };
	int set = vscl_set_message(&reply, HEAD_SYNC, blank);
	if (!set) { return 0; }

	int sent = vscl_sock_send(acceptfd, &reply);
	if (sent != 12) { return 0; }

	printf("Daemon accepted connection...\n");
	while (DAEMON_CURRENT_STATUS != TESTRIG_DAEMON_STOPPED
		   && DAEMON_CURRENT_STATUS != TESTRIG_DAEMON_CLEANING) {
		// this isn't cfg right
		int backward = vscl_sock_connect(sock, sockaddr);
		if (backward == -1) { perror("daemon connect failure"); continue; }

		vscl_byte_t buf[12] = { 0 };
#ifdef _WIN32
		int recvd = recv(backward, buf, 12, MSG_PEEK);
#else
		int recvd = read(backward, buf, 12);
#endif
		if (recvd != 12) { continue; } // TODO: some contingency?

		// TODO: proper impl that pipes this stuff (into a socket or file or stdout)
		printf("The message... %s\n", buf);
		break;
	}

	pthread_exit(&connectorstat);
	return 0;
}
// }}}

int testrig_daemon([[maybe_unused]] other_args* others) {
	char sockpath[108] = { 0 };
	int sockcopied = vscl_get_sock_destination(sockpath);
	if (sockcopied != 0) { return -1; }
	strncat(sockpath, "testrigd.sock", 14);

	struct sockaddr_un sockaddr = { .sun_family = AF_UNIX };
	strncpy(sockaddr.sun_path, sockpath, 108);

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

	delegate_info_t dinfo = {
		.sock = sock,
		.sockaddr = &sockaddr
	};

	printf("Waiting for connection...\n");
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_LISTENING;
	while (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_LISTENING) {
		int accepted = accept(sock, (struct sockaddr*)&sockaddr, &socksize);
		if (accepted == -1) { perror("daemon accept failure"); continue; }

		dinfo.acceptfd = accepted;
		int errno = pthread_create(connector, NULL, &daemon_synchronize, &dinfo);
		if (errno != 0) { perror("daemon sided synching"); }
	}

	vscl_sock_close(sock, &sockaddr);
	printf("Stopping...\n");
	return 0;
} // int testrig_daemon(other_args* others)
// vim: foldmethod=marker

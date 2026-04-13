#include <stdlib.h>
#include <stdio.h>
#include "ipc/ipc.h"
#include "epos2/identify.h"
#include "args.h"
#include "actions.h"

#ifdef _WIN32
//#include <windows.h> // holy cow there is some include order stuff with windows.h
#else
#include <unistd.h>
#include <signal.h>
#endif // _WIN32

extern enum TESTRIG_DAEMON_STATE DAEMON_CURRENT_STATUS;
extern char* action_map[];

#ifdef _WIN32
#else
static void interrupt_catcher(int sig, siginfo_t* info, void* ucontext) {
	if (sig != SIGINT || info->si_signo != SIGINT) { return; }
	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_STOPPED;
}
#endif // _WIN32

int detach_program(char** argv, enum cli_action act, const other_args* others) {
#if _WIN32
	return -1;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	char cmd[1024] = { 0 };

	strncpy(cmd, argv[0], strnlen(argv[0], 128) + 1);
	strncat(cmd, action_map[act], 10);

	for (uint8_t i = 0; i < others->size; i++) {
		strncat(cmd, " ", 2);
		strncat(cmd, others->data[i], 128);
	}

	BOOL mkdetach = CreateProcess(
			NULL, 
			cmd,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi
		);

#else
	pid_t pid = fork();

	switch (pid) {
	case -1:
		perror("Failed to fork off");
		return -1;
		break;
	case 0:
		if (setsid() == -1) { perror("Failed to detach"); return -1; }
		else { return 0; }
		break;
	default:
		return pid;
		break;
	}
#endif
}

int testrig_ident(other_args* others) {
	if (others->data == NULL) { return 1; }

	IdentifyDeviceNames();

	return 0;
}

int testrig_stat(other_args* others) {
	if (others->data == NULL) { return 1; }

	return 0;
}

// TODO: impl sigaction
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
		int synced = read(accepted, msg, 12);
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
	}

	while (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_CONNECTED) {
		// this isn't cfg right
		int accepted = SockConnect(sock, &sockaddr);
		if (accepted == -1) { perror("daemon connect failure"); continue; }

		uint8_t buf[12] = { 0 };
		int recvd = read(accepted, buf, 12);
		if (recvd != 12) { continue; } // TODO: some contingency?

		// TODO: proper impl that pipes this stuff (into a socket or file or stdout)
		printf("The message... %s\n", buf);
		break;
	}

	SockClose(sock, &sockaddr);
	printf("Stopping...\n");
	return 0;
}

int testrig_open(other_args* others) {
	if (others->data == NULL) { return 1; }

	return 0;
}

int testrig_request(other_args* others) {
	if (others->data == NULL) { return 1; }

	return 0;
}

int testrig_close(other_args* others) {
	if (others->data == NULL) { return 1; }

	return 0;
}

void free_other_args(other_args* others) {
	for (uint8_t i = 0; i < others->size; i++) {
		free(others->data[i]);
	}

	free(others->data);
}


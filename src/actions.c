#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#include "args.h"
#include "ipc/ipc.h"
#include "epos2/identify.h"
#include "actions.h"

extern enum TESTRIG_DAEMON_STATE DAEMON_CURRENT_STATUS;

int detach_program(void) {
#if _WIN32
#else
	pid_t pid = fork();

	switch (pid) {
	case -1:
		perror("Failed to fork off");
		return -1;
		break;
	case 0:
		setsid();
		return 0;
		break;
	default:
		return pid;
		break;
	}
#endif
}

int testrig_ident(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	IdentifyDeviceNames();

	return 0;
}

int testrig_stat(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	return 0;
}

int testrig_daemon(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	struct sockaddr_un sockaddr = { 0 };
	int retstat = 0;

	int sock = SockSetup(&sockaddr);
	if (sock == -1) { return -1; }

	retstat = SockBind(sock, &sockaddr);
	if (retstat == -1) { return -1; }

	retstat = SockListen(sock, 1);
	if (retstat == -1) { return -1; }

	DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_LISTENING;
	while (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_LISTENING) {
		// something that doesn't use the SockReadOut stuff because I would like to handle it
		// differently
	}

	return 0;
}

int testrig_open(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	return 0;
}

int testrig_request(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	return 0;
}

int testrig_close(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	return 0;
}

void free_other_args(other_args* others) {
	for (uint8_t i = 0; i < others->size; i++) {
		free(others->data[i]);
	}

	free(others->data);
}


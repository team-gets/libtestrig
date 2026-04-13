#include <stdlib.h>
#include <stdio.h>
#include "ipc/ipc.h"
#include "epos2/identify.h"
#include "args.h"
#include "actions.h"
#include "daemon.h"

#ifdef _WIN32
//#include <windows.h> // holy cow there is some include order stuff with windows.h
#else
#include <unistd.h>
#include <signal.h>
#endif // _WIN32

extern char* action_map[];

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


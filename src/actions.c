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
#endif // _WIN32

extern char* action_map[];

int detach_program(char** argv, enum CLI_ACTION act, const other_args* others) {
#if _WIN32
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	char cmd[1024] = { 0 };

	strncpy(cmd, argv[0], strnlen(argv[0], 128) + 1);
	strncat(cmd, " ", 2);
	strncat(cmd, action_map[act], 10);

	for (uint8_t i = 0; i < others->size; i++) {
		strncat(cmd, " ", 2);
		strncat(cmd, others->data[i], 128);
	}
	printf("%s\n", cmd);

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

	if (mkdetach == FALSE) { vscl_winprint_error("failed to detach"); return -1; }

	CloseHandle(&si);
	CloseHandle(&pi);

	return GetCurrentProcessId();

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
	if (others->data == NULL || strncmp(others->data[0], "names", 5) == 0) {
		vscl_ident_names();
		return 0;
	}

	uint8_t extra_tokens = others->size - 1;
	const char* subident = others->data[0];

	if (strncmp(subident, "protocols", 9) == 0 && extra_tokens == 1) {
		vscl_ident_protocols(others->data[1]);
		return 0;
	}
	else if (strncmp(subident, "interfaces", 12) == 0 && extra_tokens == 2) {
		vscl_ident_interfaces(others->data[1], others->data[2]);
		return 0;
	}
	else if (strncmp(subident, "ports", 6) == 0 && extra_tokens == 3) {
		vscl_ident_ports(others->data[1], others->data[2], others->data[3]);
		return 0;
	}
	else {
		fprintf(stderr, "error: the arguments passed to ident could not be parsed\n");
		return 1;
	}	
}

int testrig_stat(other_args* others) {
	if (others->data == NULL) { return 1; }

	// I'm pretty sure this requires device handles.
	// Also, wrappers haven't been made for this

	return 0;
}

int testrig_open(other_args* others) {
	if (others->data == NULL) { return 1; }

	// Launch daemon if not already, then delegate it to that

	return 0;
}

int testrig_request(other_args* others) {
	//if (others->data == NULL) { return 1; }

	// need to impl a daemon autolaunches

	struct sockaddr_un sockaddr = { 0 };
	struct sockaddr_un daemon_sockaddr = { 0 };
	int setup = vscl_sock_setup(&sockaddr);
	if (setup == -1) { return 1; }

	int not_sought = seek_daemon(&daemon_sockaddr);
	if (not_sought) { printf("not found"); vscl_sock_close(setup, &sockaddr); return 1; }

	int conn = vscl_sock_connect(setup, &daemon_sockaddr);
	if (conn == -1) { return 1; }

	struct rig_message msg = { 0 };
	vscl_set_message(&msg, HEAD_SYNC, MESSAGE_BLANK);

	// oh.. I need to impl a two-way thing
	int nbytes = vscl_sock_send(setup, &msg);
	if (nbytes == -1) { return 1; }

	return 0;
}

int testrig_close(other_args* others) {
	if (others->data == NULL) { return 1; }

	// Launch daemon if not already, then delegate it to that

	return 0;
}

void free_other_args(other_args* others) {
	for (uint8_t i = 0; i < others->size; i++) {
		free(others->data[i]);
	}

	free(others->data);
}
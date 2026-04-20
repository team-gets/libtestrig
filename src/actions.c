#include <stdlib.h>
#include <stdio.h>
#include "ipc/ipc.h"
#include "epos2/identify.h"
#include "epos2/connect.h"
#include "args.h"
#include "actions.h"
#include "daemon.h"

#ifdef _WIN32
//#include <windows.h> // holy cow there is some include order stuff with windows.h
#else
#include <unistd.h>
#endif // _WIN32

extern enum TESTRIG_DAEMON_STATE DAEMON_CURRENT_STATUS; // NOLINT
extern char* PROG_NAME; // NOLINT FIXME: WHAT IS THE READABILITIY CHECK HERE
extern char* action_map[];

static void* testrig_devices[3] = { 0 };
static struct controller testrig_controllers[3] = { 0 };

int detach_program(char** argv, enum CLI_ACTION act, const other_args* others) {
	char args[1024] = { 0 };
	sprintf(args, "%s", action_map[act]);

	for (uint8_t i = 0; i < others->size; i++) {
		strncat(args, " ", 2);
		strncat(args, others->data[i], 128);
	}

	return vscl_make_new_proc(argv[0], args);
}

int delegate_to_daemon(enum CLI_ACTION act) {
	int sock;
	struct sockaddr_un sockaddr;

	sock = vscl_sock_setup(&sockaddr);
	if (sock == INVALID_SOCKET) { return -1; }

	int found = seek_daemon(&sockaddr);
	if (!found) {
		printf("testrigd not running; creating new testrig process at ");
		int pid = vscl_make_new_proc(PROG_NAME, "--detach --daemon");
		printf("%i\n", pid);
	}

	int conn = vscl_sock_connect(sock, &sockaddr);
	if (conn == -1) { return -1; }

	struct rig_message msg = { 0 };
	vscl_byte_t head[4] = { 'W', 'A', 'N', 'T' };
	vscl_byte_t body[8] = { 0 };

	switch (act) {
	case ACTION_OPEN:
		body[0] = 'O';
		body[1] = 'P';
		body[2] = 'E';
		body[3] = 'N';
		break;
	case ACTION_CLOSE:
		body[0] = 'C';
		body[1] = 'L';
		body[2] = 'O';
		body[3] = 'S';
		body[4] = 'E';
		break;
	case ACTION_REQUEST:
		body[0] = 'R';
		body[1] = 'E';
		body[2] = 'Q';
		body[3] = 'U';
		body[4] = 'E';
		body[5] = 'S';
		body[6] = 'T';
		break;
	default:
		break;
	}

	vscl_set_message(&msg, head, body);
	vscl_sock_send(sock, &msg);
	return 0;
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

int testrig_open([[maybe_unused]] other_args* others) {
	// TODO: impl launch daemon if not already
	if (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_NONE) {
		delegate_to_daemon(ACTION_OPEN);
	}
	else {
		vscl_initialize_devices(testrig_controllers, testrig_devices, 3);
	}

	return 0;
}

int testrig_request([[maybe_unused]] other_args* others) {
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

int testrig_close([[maybe_unused]] other_args* others) {
	// Launch daemon if not already, then delegate it to that
	if (DAEMON_CURRENT_STATUS == TESTRIG_DAEMON_NONE) {
		delegate_to_daemon(ACTION_CLOSE);
	}
	else {
		vscl_close_devices(testrig_controllers, testrig_devices, 3);
	}

	return 0;
}

void free_other_args(other_args* others) {
	for (uint8_t i = 0; i < others->size; i++) {
		free(others->data[i]);
	}

	free(others->data);
}

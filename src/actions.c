#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#include "args.h"
#include "actions.h"

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
		return setsid();
		break;
	default:
		return pid;
		break;
	}
#endif
}

int testrig_ident(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	return 0;
}

int testrig_stat(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

	return 0;
}

int testrig_daemon(other_args* others) {
	if (!other_args_is_valid(others)) { return 1; }

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


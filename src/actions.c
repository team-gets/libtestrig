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

void free_other_args(other_args* others) {
	for (uint8_t i = 0; i < others->size; i++) {
		free(others->data[i]);
	}

	free(others->data);
}



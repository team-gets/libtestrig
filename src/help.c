#include <stdio.h>
#include <string.h>
#include "help.h"

static void print_hline(int width) {
	for (int i = 0; i < width; i++) {
		printf("—");
	}
	printf("\n");
}

static void printhelp_daemon(void) {
	printf("testrig daemon mode\n");
	print_hline(3);
	printf("The process serves as a mediator between the controllers\n"
		   "and other processes through an opened Unix socket,\n"
		   "continuously listening for connections and sending parsed controller data "
		   "to clients."
		   "\n");
}

int help_me(other_args* which) {
	const char* which_one = which->data[0];
	if (!strncmp("daemon", which_one, 7)) { printhelp_daemon(); return 0; }
	else { printf("read the code for help\n"); return 1; }
}

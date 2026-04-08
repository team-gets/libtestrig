#include <stdio.h>
#include <string.h>
#include "help.h"

static void print_hline(int);

int help_me(other_args* which) {
	if (which == NULL || which->size == 0) { print_usage(); return 0; };

	const char* which_one = which->data[0];

	if		(!strncmp("help", which_one, 5))	{ printhelp_help(); return 0; }
	else if (!strncmp("ident", which_one, 6))	{ printhelp_ident(); return 0; }
	else if (!strncmp("stat", which_one, 5))	{ printhelp_stat(); return 0; }
	else if (!strncmp("daemon", which_one, 7))	{ printhelp_daemon(); return 0; }
	else if (!strncmp("open", which_one, 5))	{ printhelp_open(); return 0; }
	else if (!strncmp("request", which_one, 7))	{ printhelp_request(); return 0; }
	else if (!strncmp("close", which_one, 6))	{ printhelp_close(); return 0; }
	else	{ printf("invalid target for \"help\"\n"); return 1; }
}

void print_usage(void) {
	printf("Usage: testrig [options] <action> [<args>]\n");
	print_hline(5);
	printf("Options:\n"
		   "--detach       Detach the process from the shell.\n"
		   "--daemon       Operate in daemon mode.\n"
		   "--verbose      Operate verbosely\n");
	print_hline(5);
	printf("Actions:\n"
		   "help       Print out detailed help for a specific action.\n"
		   "ident      Identify connected controllers.\n"
		   "stat       Determine the status of connected controllers.\n"
		   "open       Open connection to a device.\n"
		   "request     Request a CAN frame and print it out to stdout.\n"
		   "close      Close connection to device and release it.\n");
	printf("\n");
}

void printhelp_help(void) {
	printf("testrig help me oh mah gawd\n");
	print_hline(5);
	printf("Print help about the program or a specific action."
		   "\n");
}

void printhelp_ident(void) {
	printf("testrig ident\n");
	print_hline(5);
	printf("Identify connected devices."
		   "\n");
}

void printhelp_stat(void) {
	printf("testrig stat\n");
	print_hline(5);
	printf("Determine the status of connected devices."
		   "\n");
}

void printhelp_daemon(void) {
	printf("testrig daemon mode\n");
	print_hline(5);
	printf("The process serves as a mediator between the controllers\n"
		   "and other processes through an opened Unix socket,\n"
		   "continuously listening for connections and sending parsed controller data "
		   "to clients."
		   "\n");
}

void printhelp_open(void) {
	printf("testrig open\n");
	print_hline(5);
	printf("Open connection to the connected controller network."
		   "\n");
}

void printhelp_request(void) {
	printf("testrig request\n");
	print_hline(5);
	printf("Request a CAN frame and send it to stdout."
		   "\n");
}

void printhelp_close(void) {
	printf("testrig close\n");
	print_hline(5);
	printf("Close connection to the connected controller network and release them."
		   "\n");
}

static void print_hline(int width) {
	for (int i = 0; i < width; i++) {
		printf("—");
	}
	printf("\n");
}

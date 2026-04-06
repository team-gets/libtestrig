#pragma once

#include <stdint.h>

enum cli_mode {
	CLI_MODE_CMD,
	CLI_MODE_DETACHED
};

enum cli_action {
	/*
	 *	Print help for this program.
	 *
	 *	This is the default action.
	 */
	ACTION_HELP,

	/*
	 *	Identify nodes on the controller network.
	 */
	ACTION_IDENT,

	/*
	 *	Print out statuses of contoller(s on the network)
	 */
	ACTION_STAT,

	/*
	 *	Operate in daemon mode,
	 *	serving as a mediator between the controllers
	 *	and programs at the other end of a socket
	 */
	ACTION_DAEMON,

	/*
	 *	Open communications to the controller network.
	 */
	ACTION_OPEN,

	/*
	 *	Request a CAN frame and record it to stdout.
	 */
	ACTION_RECORD,
	
	/*
	 *	Close communications to the controller network.
	 */
	ACTION_CLOSE,
};

typedef struct {
	uint8_t size;
	char** data;
} other_args;

struct parsed_args {
	enum cli_mode mode;
	enum cli_action action;
	uint8_t verbosity;

	// Operate on the passed immmutable strings.
	int(*fun)(char**);
};

int is_flag(const char* arg);
int is_opt(const char* arg);

int parse_flag(const char* flag, struct parsed_args* parsed);
int parse_opt(const char* opt, struct parsed_args* parsed);
int parse_act(const char* act, struct parsed_args* parsed);
void parse_args(int argc, char** argv, struct parsed_args* parsed, other_args* others);

#pragma once

#include <stdint.h>
#include "other_args.h"
#include "actions.h"

enum cli_mode {
	CLI_MODE_CMD = 0,
	CLI_MODE_DETACHED = 1
};

enum cli_action {
	/*
	 *	Print help for this program.
	 *
	 *	This is the default action.
	 */
	ACTION_HELP = 0,

	/*
	 *	Identify nodes on the controller network.
	 */
	ACTION_IDENT = 1,

	/*
	 *	Print out statuses of contoller(s on the network)
	 */
	ACTION_STAT = 2,

	/*
	 *	Operate in daemon mode,
	 *	serving as a mediator between the controllers
	 *	and programs at the other end of a socket
	 */
	ACTION_DAEMON = 3,

	/*
	 *	Open communications to the controller network.
	 */
	ACTION_OPEN = 4,

	/*
	 *	Request a CAN frame and record it to stdout.
	 */
	ACTION_RECORD = 5,
	
	/*
	 *	Close communications to the controller network.
	 */
	ACTION_CLOSE = 6,
};

struct parsed_args {
	enum cli_mode mode;
	enum cli_action action;
	uint8_t verbosity;

	// Operate on the passed immmutable strings.
	int(*fun)(other_args**);
};

int is_flag(const char* arg);
int is_opt(const char* arg);

int parse_flag(const char* flag, struct parsed_args* parsed);
int parse_opt(const char* opt, struct parsed_args* parsed);
int parse_act(const char* act, struct parsed_args* parsed);
void parse_args(int argc, char** argv, struct parsed_args* parsed, other_args* others);
int validate_combo(enum cli_mode mode, enum cli_action action);

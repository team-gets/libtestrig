#pragma once

#include <stdint.h>

enum cli_mode {
	CLI_MODE_CMD,
	CLI_MODE_DETACHED
};

enum cli_action {
	ACTION_HELP,
	ACTION_IDENT,
	ACTION_DAEMON,
	ACTION_OPEN,
	ACTION_RECORD,
	ACTION_CLOSE,
};

struct parsed_args {
	enum cli_mode mode;
	enum cli_action action;
	uint8_t verbosity;
};

int is_flag(const char* arg);
int is_opt(const char* arg);

int parse_flag(const char* flag, struct parsed_args* parsed);
void parse_args(int argc, char** argv, struct parsed_args* parsed);

#pragma once

enum cli_mode {
	CLI_MODE_CMD,
	CLI_MODE_DETACHED
};

enum cli_action {
	ACTION_IDENT,
	ACTION_DAEMON,
	ACTION_OPEN,
	ACTION_RECORD,
	ACTION_CLOSE,
};

struct parsed_args {
	enum cli_mode mode;
	enum cli_action action;
};

int is_flag(const char* arg);
int is_opt(const char* arg);
void parse_args(int argc, char** argv, struct parsed_args* parsed);

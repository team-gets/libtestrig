#pragma once

enum cli_mode {
	CLI_MODE_CMD,
	CLI_MODE_DETACHED
};

enum cli_action {
	ACTION_IDENT,
};

struct parsed_args {
	enum cli_mode mode;
	enum cli_action action;
};

void parse_args(int argc, char** argv, struct parsed_args* parsed);

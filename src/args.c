#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args.h"

static void die_invalid_arg(const char* arg) {
	printf("error: invalid argument \"%s\"\n", arg);
	exit(-1);
}

int is_flag(const char* arg) { return (strstr(arg, "--") == NULL) ? 0 : 1; }
int is_opt(const char* arg) {
	char* subarg;
	if ((subarg = strstr(arg, "-"))) {
		return (strstr(subarg, "-")) ? 1 : 0;
	}
	else {
		return 0;
	}
}

int parse_flag(const char* flag, struct parsed_args* parsed) {
	const char* name = strstr(flag, "--");
	
	if (strncmp("daemon", name, 7) == 0) {
		parsed->action = ACTION_DAEMON;
		return 1;
	}
	else if (strncmp("detach", name, 7) == 0) {
		parsed->mode = CLI_MODE_DETACHED;
		return 1;
	}
	else if (strncmp("verbose", name, 8) == 0) {
		parsed->verbosity = 1;
		return 1;
	}
	else {
		return 0;
	}
}

void parse_args(int argc, char** argv, struct parsed_args* parsed) {
	memset(parsed, 0, sizeof(struct parsed_args));
	parsed->mode = CLI_MODE_CMD;
	parsed->action = ACTION_HELP;

	for (unsigned int i = 1; i < (unsigned int)argc; i++) {
		char* arg = argv[i];
		int is_a_flag = is_flag(arg);
		int is_an_opt = is_opt(arg);
		int result;
		
		if (is_a_flag) {
			result = parse_flag(arg, parsed);
		}
		else if (is_an_opt) {
			result = 1;
		}
		else {
			result = 1;
		}

		if (!result) { die_invalid_arg(arg); }
	}
}

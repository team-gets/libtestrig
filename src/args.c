#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "help.h"
#include "args.h"

static void die_invalid_arg(const char* arg) {
	printf("error: invalid argument \"%s\"\n", arg);
	exit(-1);
}

static char* mode_map[] = { "command", "detach" };
static char* action_map[] = { "help", "ident", "status", "open", "record", "close" };
static int(*fun_map[])(other_args*) = {
	&help_me, &testrig_ident, &testrig_stat, &testrig_open, &testrig_record, &testrig_close };

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
	const char* name = strstr(flag, "--") + 2;
	int ret = 0;

	if		(!strncmp("daemon", name, 7))	{ parsed->action = ACTION_DAEMON; ret = 1; }
	else if (!strncmp("detach", name, 7)) 	{ parsed->mode = CLI_MODE_DETACHED; ret = 1; }
	else if (!strncmp("verbose", name, 8))	{ parsed->verbosity = 1; ret = 1; }
	else if (!strncmp("help", name, 5))		{ parsed->action = ACTION_HELP; ret = 1; }

	return ret;
}

int parse_act(const char* act, struct parsed_args* parsed) {
	int ret = 0;

	if		(!strncmp("daemon", act, 7))	{ parsed->action = ACTION_DAEMON; ret = 1; }
	else if (!strncmp("ident", act, 6))		{ parsed->action = ACTION_IDENT; ret = 1; }
	else if (!strncmp("status", act, 7))	{ parsed->action = ACTION_STAT; ret = 1; }
	else if (!strncmp("open", act, 5))		{ parsed->action = ACTION_OPEN; ret = 1; }
	else if (!strncmp("record", act, 7))	{ parsed->action = ACTION_RECORD; ret = 1; }
	else if (!strncmp("close", act, 6))		{ parsed->action = ACTION_CLOSE; ret = 1; }
	else if (!strncmp("help", act, 5))		{ parsed->action = ACTION_HELP; ret = 1; }

	return ret;
}

int parse_opt(const char* arg, struct parsed_args* parsed) {
	const char* opts = strstr(arg, "-");
	int ret = 0;
	int i = 0;
	char opt = opts[i];

	while (opt != 0) {
		switch (opt) {
		case 'v':
			parsed->verbosity = 1;
			ret = 1;
			break;
		}

		opt = opts[i];
		i++;
	}

	return ret;
}

int validate_combo(enum cli_mode mode, enum cli_action action) {
	switch (mode) {
	case CLI_MODE_DETACHED:
		switch (action) {
		case ACTION_DAEMON:
		case ACTION_HELP:
			return 1;
		default:
			return 0;
		}
	case CLI_MODE_CMD:
	default:
		return 1;
	}
}

void parse_args(int argc, char** argv, struct parsed_args* parsed, other_args* others) {
	memset(parsed, 0, sizeof(struct parsed_args));
	parsed->mode = CLI_MODE_CMD;
	parsed->action = ACTION_HELP;
	int action_set = 0;

	for (unsigned int i = 1; i < (unsigned int)argc; i++) {
		const char* arg = argv[i];
		int is_a_flag = is_flag(arg);
		int is_an_opt = is_opt(arg);
		int result;
		
		if (is_a_flag) {
			result = parse_flag(arg, parsed);
		}
		else if (is_an_opt) {
			result = parse_opt(arg, parsed);
		}
		else {
			if (!action_set) {
				result = parse_act(arg, parsed);
				action_set = 1;
			}
			else {
				size_t strsize = strnlen(arg, 255) + 1;
				size_t idx = others->size;
				others->size++;

				others->data = (char**)realloc(others->data, others->size * sizeof(char*));
				others->data[idx] = (char*)malloc(strsize);

				strncpy(others->data[idx], arg, strsize);
				result = 1;
			}
		} // if ()

		if (!result) { die_invalid_arg(arg); }
	} // for ()

	parsed->fun = fun_map[parsed->action];
	if (!validate_combo(parsed->mode, parsed->action)) {
		printf("Invalid combination: %s and %s\n", mode_map[parsed->mode], action_map[parsed->action]);
		exit(-1);
	}
} // void parse_args(int argc, char** argv, struct parsed_args* parsed, other_args* others)

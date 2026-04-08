#include <stdio.h>
#include <stdlib.h>

#include "ipc/ipc.h"
#include "epos2/epos2.h"

#include "actions.h"
#include "help.h"
#include "args.h"

enum TESTRIG_DAEMON_STATE DAEMON_CURRENT_STATUS = TESTRIG_DAEMON_NONE;

int main(int argc, char** argv) {
	if (argc < 2 || argc > 255) {
		print_usage();
		return -1;
	}

	struct parsed_args parsed;
	other_args others = { 0, 0 };
	parse_args(argc, argv, &parsed, &others);
	int ret = 0;

	switch (parsed.mode) {
	case CLI_MODE_DETACHED:
		ret = detach_program();
		break;
	case CLI_MODE_CMD:
	default:
		break;
	}

	if (parsed.fun == NULL) {
		printf("unimplemented action\n");
	}
	else if (ret == 0) {
		ret = parsed.fun(&others);
	}

	free_other_args(&others);

	return ret;
}

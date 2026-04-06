#include <stdio.h>
#include <stdlib.h>

#include "ipc/ipc.h"
#include "epos2/epos2.h"

#include "actions.h"
#include "args.h"

int main(int argc, char** argv) {
	if (argc < 2 || argc > 255) {
		printf("Usage: testrig <options>\n");
		return -1;
	}

	struct parsed_args parsed;
	other_args others = { 0, 0 };
	parse_args(argc, argv, &parsed, &others);

	int detach;
	switch (parsed.mode) {
	case CLI_MODE_DETACHED:
		detach = detach_program();
		break;
	case CLI_MODE_CMD:
	default:
		break;
	}

	for (uint8_t i = 0; i < others.size; i++) {
		printf("The others were %s at loc %u\n", others.data[i], i);
	}

	free_other_args(&others);

	return 0;
}

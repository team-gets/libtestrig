#include <stdio.h>
#include <stdlib.h>

#include "ipc/ipc.h"
#include "epos2/epos2.h"
#include "args.h"

int main(int argc, char** argv) {
	if (argc < 2 || argc > 255) {
		printf("Usage: testrig <options>\n");
		return -1;
	}

	struct parsed_args parsed;
	other_args others = { 0, 0 };
	parse_args(argc, argv, &parsed, &others);

	for (uint8_t i = 0; i < others.size; i++) {
		printf("The others were %s at loc %u\n", others.data[i], i);
	}

	for (uint8_t i = 0; i < others.size; i++) {
		free(others.data[i]);
	}
	free(others.data);

	return 0;
}

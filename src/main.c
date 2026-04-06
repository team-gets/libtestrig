#include <stdio.h>

#include "ipc/ipc.h"
#include "epos2/epos2.h"
#include "args.h"

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: testrig <options>\n");
		return -1;
	}

	struct parsed_args parsed;
	parse_args(argc, argv, &parsed);

	return 0;
}

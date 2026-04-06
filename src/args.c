#include <stdio.h>
#include <string.h>
#include "args.h"

int is_flag(const char* arg) { return (strstr(arg, "--") == NULL) ? 0 : 1; }
int is_opt(const char* arg) {
	char* subarg;
	if ((subarg = strstr(arg, "-"))) {
		return (strstr(subarg, "-") == NULL) ? 0 : 1;
	}
	else {
		return 0;
	}
}

void parse_args(int argc, char** argv, struct parsed_args* parsed) {
	for (unsigned int i = 1; i < (unsigned int)argc; i++) {
		int is_a_flag = is_flag(argv[i]);
		int is_an_opt = is_opt(argv[i]);
		
		if (is_a_flag) {

		}
		else if (is_an_opt) {

		}
		else {

		}
	}
}

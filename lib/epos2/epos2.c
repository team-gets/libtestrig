#include <stdio.h>
#include "epos2.h"

const char* DEFAULT_NAME = "EPOS2";

int DecodeError(const uint32_t error_code, char* error_msg, uint8_t max_size) {
	int ret = VCS_GetErrorInfo(error_code, error_msg, max_size);
	if (ret == 0) { printf("ERROR: Failed to decode error\n"); }

	return ret;
}

int PrintError(const uint32_t error_code) {
	char msg[64] = { 0 };
	int ret = DecodeError(error_code, msg, 64);
	printf("ERROR 0x%X: %s\n", error_code, msg);

	return ret;
}

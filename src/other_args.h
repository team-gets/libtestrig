#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	uint8_t size;
	char** data;
} other_args;

static int other_args_is_valid(other_args* others) {
	return (others == NULL || others->size == 0) ? 0 : 1;
}

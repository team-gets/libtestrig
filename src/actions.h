#pragma once

#include <stdint.h>
#include "other_args.h"
#include "args.h"

int detach_program(void);
void free_other_args(other_args* others);

/*
 *	Actions methods
 *
 *	This is the end, so it returns what a program would.
 *	0 (success) or 1 (failure)
 */

#pragma once

#include <stdint.h>
#include "other_args.h"
#include "args.h"

int detach_program(char** argv, enum cli_action act, const other_args* others);

int testrig_ident(other_args* others);
int testrig_stat(other_args* others);
int testrig_open(other_args* others);
int testrig_request(other_args* others);
int testrig_close(other_args* others);

void free_other_args(other_args* others);

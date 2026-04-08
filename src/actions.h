/*
 *	Actions methods
 *
 *	Returns what a program would.
 *	0 (success) or 1 (failure)
 */

#pragma once

#include <stdint.h>
#include "other_args.h"
#include "args.h"

int detach_program(void);

int testrig_ident(other_args* others);
int testrig_stat(other_args* others);
int testrig_daemon(other_args* others);
int testrig_open(other_args* others);
int testrig_request(other_args* others);
int testrig_close(other_args* others);

void free_other_args(other_args* others);

enum TESTRIG_DAEMON_STATE {
	TESTRIG_DAEMON_NONE = 1,
	TESTRIG_DAEMON_LISTENING = 2,
	TESTRIG_DAEMON_CLEANING = 3,
	TESTRIG_DAEMON_STOPPED = 4
};

#pragma once

#include "ipc/ipc.h"
#include "other_args.h"

int testrig_daemon(other_args* others);
int seek_daemon(struct sockaddr_un* sockaddr);
void* daemon_synchronize(void* arg);

enum TESTRIG_DAEMON_STATE {
	TESTRIG_DAEMON_NONE = 1,
	TESTRIG_DAEMON_LISTENING = 2,
	TESTRIG_DAEMON_CLEANING = 4,
	TESTRIG_DAEMON_STOPPED = 8,
	TESTRIG_DAEMON_CONNECTED = 16
};

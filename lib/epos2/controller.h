#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "libtestrig_api.h"

enum CONTROLLER_STATE {
	CTRL_STATE_CLOSED = 1,
	CTRL_STATE_OPENED = 2,
};

/*
 *	@brief A struct representing a controller.
 *
 *	Refer to the parameters passed to VCS_OpenDevice, Page 13 of the Manual.
 */
struct controller {
	// The name of the controller.
	char name[8];
	// The name of the communication protocol.
	char protocol[16];
	// The name of the connection interface.
	char conn_interface[64];
	// The name of the port used in connection.
	char port[8];
	// The node of the controller.
	vscl_byte_t node_id;
	// State of the controller.
	enum CONTROLLER_STATE state;
};

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

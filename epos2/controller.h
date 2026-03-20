#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>

#ifdef __cplusplus
} // extern "C" (headers)
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

enum ACQUIRE_DEVICE_INFORMATION_FLAGS {
	FLAG_INIT_DEFAULTS = 1,
	FLAG_ZERO_INIT = 2,
};

enum ControllerState {
	CTRL_STATE_CLOSED = 1,
	CTRL_STATE_OPENED = 2,
};

/*
 *	@brief A struct representing a controller.
 *
 *	Refer to the parameters passed to VCS_OpenDevice, Page 13 of the Manual.
 */
struct Controller {
	// The name of the controller.
	char Name[8];
	// The name of the communication protocol.
	char Protocol[16];
	// The name of the connection interface.
	char Interface[64];
	// The name of the port used in connection.
	char Port[8];
	// The node of the controller.
	uint8_t NodeId;
	// State of the controller.
	enum ControllerState State;
};

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

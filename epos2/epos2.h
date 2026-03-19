/*
 *	a planning doc
 *
 */

#pragma once

#ifdef __cplusplus
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

struct Controller {
	char Name[8];
	char Protocol[16];
	char Interface[64];
	char Port[8];
};

enum ACQUIRE_DEVICE_INFORMATION_FLAGS {
	FLAG_INIT_DEFAULTS = 1,
	FLAG_ZERO_INIT = 2
};

/*
 *	Initializes the device and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual.
 */
unsigned int InitializeDevice(struct Controller* controller, void* device_handle);

/*
 *	Populates the fields of the Controller information struct.
 *
 *	Flags are passed bitwise ORed.
 */
unsigned int AcquireDeviceInformation(struct Controller* controller, int flags);

/*
 *	Closes all connections to the controller
 */
unsigned int CloseDevice(struct Controller* controller, void* device_handle);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

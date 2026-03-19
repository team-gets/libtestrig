#pragma once

#ifdef __cplusplus
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

#include "libtestrig_api.h"

struct Controller {
	char Name[8];
	char Protocol[16];
	char Interface[64];
	char Port[8];
	unsigned short NodeId;
};

enum ACQUIRE_DEVICE_INFORMATION_FLAGS {
	FLAG_INIT_DEFAULTS = 1,
	FLAG_ZERO_INIT = 2
};

/*
 *	Populates the fields of the Controller information struct.
 *
 *	Flags are passed bitwise ORed.
 */
unsigned int TESTRIG_API AcquireDeviceInformation(struct Controller* controller, int flags);

/*
 *
 */
unsigned int TESTRIG_API AcquireThreeDeviceInformations(struct Controller* controller, int flags);

/*
 *	Initializes the device and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual.
 */
unsigned int TESTRIG_API InitializeDevice(struct Controller* controller, void* device_handle);

/*
 *	Closes all connections to the controller
 */
unsigned int TESTRIG_API CloseDevice(struct Controller* controller, void* device_handle);

/*
 *	Print out controller characteristics.
 */
void TESTRIG_API PrintControllerCharacteristics(struct Controller* controller);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

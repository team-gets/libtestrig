#pragma once

#ifdef __cplusplus
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

#include "libtestrig_api.h"
#include "controller.h"

/*
 *	@brief Opens communication to the device and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual under INITIALIZATION,
 *	which describes several of the methods wrapped here.
 */
unsigned int TESTRIG_API InitializeDevice(struct Controller* controller, void* device_handle);

/*
 *	Closes all connections to the controller
 *
 *	Refer to Page 162 of the manual under CLOSING PROCEDURE,
 *	which describes several of the methods wrapped here.
 */
unsigned int TESTRIG_API CloseDevice(struct Controller* controller, void* device_handle);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

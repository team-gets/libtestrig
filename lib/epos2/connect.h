#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "libtestrig_api.h"
#include "controller.h"

#ifdef __cplusplus
} // extern "C" (headers)
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

uint32_t CleanEnableDevice(struct Controller* controller, void* device_handle);

/*
 *	@brief Opens communication to the device and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual under INITIALIZATION,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API InitializeDevices(struct Controller** controllers_out, void* node_1, void* node_2, void* node_3);

/*
 *	Closes all connections to the controller
 *
 *	Refer to Page 162 of the manual under CLOSING PROCEDURE,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API CloseDevice(struct Controller* controller, void* device_handle);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

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

uint32_t TESTRIG_API CleanEnableDevice(struct Controller* controller, void* device_handle);

/*
 *	@brief Opens communication to the device and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual under INITIALIZATION,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API InitializeDevice(struct Controller* controller_out, void* node, uint8_t node_id);

/*
 *	@brief Opens communication to the devices in a gateway configuraton,
 *	and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual under INITIALIZATION,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API InitializeDevices(
	struct Controller controllers_out[], void* handles_out[], uint8_t num);

/*
 *	@brief Closes all connections to a single controller
 *
 *	Refer to Page 162 of the manual under CLOSING PROCEDURE,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API CloseDevice(struct Controller* controller, void* device_handle);

/*
 *	@brief Closes connections to multiple controllers in a gateway configuration,
 *	assuming the first handle and controller is the gateway.
 */
uint32_t TESTRIG_API CloseDevices(
		struct Controller controllers[], void* device_handles[], uint8_t num);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

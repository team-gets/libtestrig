#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "libtestrig_api.h"
#include "controller.h"

uint32_t TESTRIG_API vscl_clean_enable_device(struct controller* controller, void* device_handle);

/*
 *	@brief Opens communication to the device and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual under INITIALIZATION,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API vscl_initialize_device(struct controller* controller_out, void* node, vscl_byte_t node_id);

/*
 *	@brief Opens communication to the devices in a gateway configuraton,
 *	and sets its state for a clean init.
 *
 *	Refer to Page 162 of the manual under INITIALIZATION,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API vscl_initialize_devices(
	struct controller controllers_out[], void* handles_out[], vscl_byte_t num);

/*
 *	@brief Closes all connections to a single controller
 *
 *	Refer to Page 162 of the manual under CLOSING PROCEDURE,
 *	which describes several of the methods wrapped here.
 */
uint32_t TESTRIG_API vscl_close_device(struct controller* controller, void* device_handle);

/*
 *	@brief Closes connections to multiple controllers in a gateway configuration,
 *	assuming the first handle and controller is the gateway.
 */
uint32_t TESTRIG_API vscl_close_devices(
		struct controller controllers[], void* device_handles[], vscl_byte_t num);

#ifdef __cplusplus
} // extern "C"
#endif

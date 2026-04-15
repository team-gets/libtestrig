#pragma once

#include "libtestrig_api.h"
#include "definitions.h"
#include "controller.h"
#include "identify.h"
#include "connect.h"
#include "manage.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern const char* default_name;

/*
 *	@brief Decode a given error code to its corresponding message.
 */
int TESTRIG_API vscl_decode_error(const uint32_t error_code, char* error_msg, uint8_t max_size);

/*
 *	@brief Decode and print an error code's corresponding message.
 */
int TESTRIG_API vscl_print_error(const uint32_t error_code);

/*
 *	@brief Reset a device.
 */
uint32_t TESTRIG_API vscl_reset_device(void* device_handle, struct controller* controller_in);

/*
 *	@brief Setup that's tooled to our configuration.
 */
uint32_t TESTRIG_API vscl_setup_testrig_as_can_gateway(
	struct controller controllers[3], void* handles[3], int default_init);

/*
 *	@brief Cleanup that's tooled to our configuration.
 */
uint32_t TESTRIG_API vscl_cleanup_testrig(
	struct controller controllers[3], void* handles[3]);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#pragma once

#include "libtestrig_api.h"
#include "definitions.h"
#include "controller.h"
#include "identify.h"
#include "connect.h"
#include "manage.h"

#ifdef __cplusplus
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

extern const char* DEFAULT_NAME;

/*
 *	@brief Decode a given error code to its corresponding message.
 */
int TESTRIG_API DecodeError(const uint32_t error_code, char* error_msg, uint8_t max_size);

/*
 *	@brief Decode and print an error code's corresponding message.
 */
int TESTRIG_API PrintError(const uint32_t error_code);

/*
 *	@brief Reset a device.
 */
uint32_t TESTRIG_API ResetDevice(void* device_handle, struct Controller* controller_in);

/*
 *	@brief Setup that's tooled to our configuration.
 */
uint32_t TESTRIG_API SetupTestrigAsCANGateway(
	struct Controller controllers[3], void* handles[3], int default_init);

/*
 *	@brief Cleanup that's tooled to our configuration.
 */
uint32_t TESTRIG_API CleanupTestrig(
	struct Controller controllers[3], void* handles[3]);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include "libtestrig_api.h"
#include "controller.h"

#ifdef __cplusplus
} // extern "C" (headers)
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

enum ACQUIRE_DEVICE_INFORMATION_FLAGS {
	FLAG_ACQUIRE_INIT_DEFAULTS = 1,
	FLAG_ACQUIRE_ZERO_INIT = 2,
};

/*
 *	@brief Prints out potential device names.
 *
 *	Refer to Page 3-25 of the manual.
 */
uint32_t TESTRIG_API IdentifyDeviceNames(void);

/*
 *	@brief Finds the protocol stacks associated with the device name.
 *	This passed name is not mutated.
 *
 *	Refer to Page 3-26 of the manual.
 */
uint32_t TESTRIG_API IdentifyDeviceProtocols(char* device_name);

/*
 *	@brief Finds the interfaces associated with the device name and protocol.
 *	Passed fields are not mutated.
 *
 *	Refer to Page 3-27 of the manual.
 */
uint32_t TESTRIG_API IdentifyDeviceInterfaces(char* device_name, char* protocol_stack);

/*
 *	@brief Finds the ports assoiated with the device name, protocol, and interface.
 *
 *	Refer to Page 3-28 of the manual.
 */
uint32_t TESTRIG_API IdentifyDevicePorts(char* device_name, char* protocol_stack, char* device_interface);

/*
 *	@brief Print out controller characteristics.
 */
void TESTRIG_API PrintControllerCharacteristics(struct Controller* controller_out);


#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

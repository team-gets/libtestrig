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
 *	@brief Finds the name of the Controllers in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-25 of the manual.
 */
uint32_t TESTRIG_API AcquireDeviceNames(struct Controller** controllers_out, int size);

/*
 *	@brief Finds the protocol stacks of the Controller in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-26 of the manual.
 */
uint32_t TESTRIG_API AcquireDeviceProtocols(struct Controller** controllers_out, int size);

/*
 *	@brief Finds the interfaces of the Controller in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-27 of the manual.
 */
uint32_t TESTRIG_API AcquireDeviceInterfaces(struct Controller** controllers_out, int size);

/*
 *	@brief Finds the ports of the Controller in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-28 of the manual.
 */
uint32_t TESTRIG_API AcquireDevicePorts(struct Controller** controllers_out, int size);

/*
 *	@brief Acquire the information on the set number of controllers.
 */
uint32_t TESTRIG_API AcquireDeviceInfos(struct Controller** controllers_out, int size, int flags);

/*
 *	@brief Set the controller to be identified by the given node on the network.
 *
 *	Refer to 8-135 of the firmware manual. An unconfigured node is found at node 0.
 */
uint32_t TESTRIG_API SetDeviceNode(struct Controller* controller_out, void* handle, uint8_t old_node, uint8_t new_node);

/*
 *	@brief Print out controller characteristics.
 */
void TESTRIG_API PrintControllerCharacteristics(struct Controller* controller_out);


#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

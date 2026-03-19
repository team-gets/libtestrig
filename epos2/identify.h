#pragma once

#include "libtestrig_api.h"
#include "controller.h"

#ifdef __cplusplus
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

/*
 *	@brief Populates the fields of the Controller information struct.
 *	This only operates on the first controller that the Command Library finds.
 *
 *	Flags are passed bitwise ORed.
 *
 *	Refer to Page 162 of the manual under HELP,
 *	which describes several of the functions wrapped here.
 */
unsigned int TESTRIG_API AcquireFirstDeviceInformation(struct Controller* controller_out, int flags);

/*
 *	@brief Finds the name of the Controllers in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-25 of the manual.
 */
unsigned int TESTRIG_API AcquireDeviceNames(struct Controller** controllers_out, int size);

/*
 *	@brief Finds the protocol stacks of the Controller in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-26 of the manual.
 */
unsigned int TESTRIG_API AcquireDeviceProtocols(struct Controller** controllers_out, int size);

/*
 *	@brief Finds the interfaces of the Controller in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-27 of the manual.
 */
unsigned int TESTRIG_API AcquireDeviceInterfaces(struct Controller** controllers_out, int size);

/*
 *	@brief Finds the ports of the Controller in the array.
 *	This operates on all connected controllers.
 *
 *	Refer to Page 3-28 of the manual.
 */
unsigned int TESTRIG_API AcquireDevicePorts(struct Controller** controllers_out, int size);

/*
 *	@brief Acquire the three controllers.
 */
unsigned int TESTRIG_API AcquireThreeDeviceInformations(struct Controller** controllers_out, int flags);

/*
 *	@brief Print out controller characteristics.
 */
void TESTRIG_API PrintControllerCharacteristics(struct Controller* controller_out);


#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

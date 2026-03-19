#pragma once

#include "libtestrig_api.h"
#include "controller.h"

#ifdef __cplusplus
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

/*
 *	@brief Populates the fields of the Controller information struct.
 *
 *	Flags are passed bitwise ORed.
 *
 *	Refer to Page 162 of the manual under HELP,
 *	which describes several of the functions wrapped here.
 */
unsigned int TESTRIG_API AcquireDeviceInformation(struct Controller* controller, int flags);

/*
 *	@brief Acquire the three controllers.
 */
unsigned int TESTRIG_API AcquireThreeDeviceInformations(struct Controller** controllers, int flags);

/*
 *	@brief Print out controller characteristics.
 */
void TESTRIG_API PrintControllerCharacteristics(struct Controller* controller);


#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

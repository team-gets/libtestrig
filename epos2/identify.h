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

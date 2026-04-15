#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "epos2.h"
#include "controller.h"


/*
 *	Attempt to abort the test rig.
 */
uint32_t TESTRIG_API vscl_rig_abort(const struct controller* controller_in, void* device_handle);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

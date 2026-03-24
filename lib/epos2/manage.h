#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "epos2.h"
#include "controller.h"

#ifdef __cplusplus
} // extern "C" (headers)
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

uint32_t TESTRIG_API RigAbort(const struct Controller* controller_in, void* device_handle);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

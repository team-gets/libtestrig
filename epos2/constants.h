#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef __cplusplus
}
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

enum ACQUIRE_DEVICE_INFORMATION_FLAGS {
	FLAG_INIT_DEFAULTS = 1,
	FLAG_ZERO_INIT = 2
};

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

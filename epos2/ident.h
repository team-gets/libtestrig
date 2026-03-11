#pragma once

#ifdef __cplusplus
namespace VSCL::Rig {
extern "C" {
#endif // __cplusplus

struct Controller {
	char Name[8];
	char Protocol[16];
	char Interface[64];
	char Port[8];
};

/*
 *	Populates the fields of the Controller information struct.
 *
 *	Flags are passed bitwise ORed.
 */
int AcquireDeviceInformation(struct Controller* controller, int flags);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::Rig
#endif // __cplusplus

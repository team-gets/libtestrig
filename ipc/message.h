#pragma once

// Headers
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "vscl_export.h"

#ifdef __cplusplus
} // extern "C"
namespace VSCL {
extern "C" {
#endif // __cplusplus

/*
 *	A test rig message.
 *	These are unsigned chars - but more explict - on the architectures and OSes we care about
 *	(x86_64 Linux and Windows, basically whatever Maxon supports)
 */ 
VSCL_EXPORT struct RigMessage {
	// Header: Four bytes to avoid collisions while keeping messages small.
	uint8_t head[4];

	// Data: Eight bytes as described by the size of the data part on a(n EPOS2) CAN frame.
	uint8_t data[8];
};

// Convenience function that encapsulates copying to the header and data fields.
VSCL_EXPORT int SetMessage(struct RigMessage* msg, const uint8_t* head, const uint8_t* data);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL
#endif

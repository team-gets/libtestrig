#pragma once

// Headers
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "libtestrig_api.h"

/*
 *	A test rig message.
 *	These are unsigned chars - but more explict - on the architectures and OSes we care about
 *	(x86_64 Linux and Windows, basically whatever Maxon supports)
 */ 
TESTRIG_API struct rig_message {
	// Header: Four bytes to avoid collisions while keeping messages small.
	uint8_t head[4];

	// Data: Eight bytes as described by the size of the data part on a(n EPOS2) CAN frame.
	uint8_t data[8];
};

/* 
 *	Signal to the listener process to continue listening on the socket.
 *
 *	The listener process does not accept messages without this header.
 */
TESTRIG_API extern const uint8_t HEAD_STAY[4];

/*
 *	Signal to the listener process to stop listening on the socket and close.
 */
TESTRIG_API extern const uint8_t HEAD_DC[4];

/*
 *	Synchronize message header. Sent back and forth to confirm connection.
 */
TESTRIG_API extern const uint8_t HEAD_SYNC[4];

/*
 *	Synchronize message header. Sent back and forth to confirm connection.
 */
TESTRIG_API extern const uint8_t HEAD_IDENT[4];

/*
 *	Blank message.
 */
TESTRIG_API extern const uint8_t MESSAGE_BLANK[8];

// Convenience function that encapsulates copying to the header and data fields.
TESTRIG_API int vscl_set_message(struct rig_message* msg, const uint8_t* head, const uint8_t* data);

/*
 *	The type of header.
 */
TESTRIG_API enum HEADER_TYPE {
	HEADER_IS_STAY = 1,
	HEADER_IS_DC = 2,
	HEADER_IS_SYNC = 4,
};


#ifdef __cplusplus
} // extern "C"
#endif

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "libtestrig_api.h"

#ifdef __cplusplus
}
namespace VSCL::IPC {
extern "C" {
#endif // __cplusplus

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
 *	Socket read flag
 *
 *	When receiving HEAD_DC, disconnect right after the client does.
 */
TESTRIG_API extern const int DC_WITH_CLIENT;

/*
 *	The type of header.
 */
TESTRIG_API enum HEADER_TYPE {
	HEADER_IS_STAY = 1,
	HEADER_IS_DC = 2
};

/*
 *	For use with socket handler functions.
 */
TESTRIG_API enum HEADER_RET {
	// When receiving HEAD_DC, disconnect right after the client does.
	HANDLER_RET_STAY = 1,
	HANDLER_RET_DC = 2
};

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL::IPC
#endif // __cplusplus

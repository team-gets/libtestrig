#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "vscl_export.h"

#ifdef __cplusplus
}
namespace VSCL {
extern "C" {
#endif // __cplusplus

/* 
 *	Signal to the listener process to continue listening on the socket.
 *
 *	The listener process does not accept messages without this header.
 */
VSCL_API extern const uint8_t HEAD_STAY[4];

/*
 *	Signal to the listener process to stop listening on the socket and close.
 */
VSCL_API extern const uint8_t HEAD_DC[4];

/*
 *	Socket read flag
 *
 *	When receiving HEAD_DC, disconnect right after the client does.
 */
VSCL_API extern const int DC_WITH_CLIENT;

/*
 *	The type of header.
 */
VSCL_API enum HEADER_TYPE {
	HEADER_IS_STAY = 1,
	HEADER_IS_DC = 2
};

/*
 *	For use with socket handler functions.
 */
VSCL_API enum HEADER_RET {
	// When receiving HEAD_DC, disconnect right after the client does.
	HANDLER_RET_STAY = 1,
	HANDLER_RET_DC = 2
};

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL
#endif // __cplusplus

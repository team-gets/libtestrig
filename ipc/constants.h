#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

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
extern const uint8_t HEAD_STAY[4];

/*
 *	Signal to the listener process to stop listening on the socket and close.
 */
extern const uint8_t HEAD_DC[4];

/*
 *	Socket read flag
 *
 *	When receiving HEAD_DC, disconnect right after the client does.
 */
extern const int DC_WITH_CLIENT;

/*
 *	For use with socket handler functions.
 *
 *	When receiving HEAD_DC, disconnect right after the client does.
 */
extern const int HANDLER_RET_DC;

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL
#endif // __cplusplus

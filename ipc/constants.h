#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifdef __cplusplus
}
namespace Gets {
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
extern const uint8_t HEAD_DISCONNECTING[4];

#ifdef __cplusplus
} // extern "C"
} // namespace Gets
#endif // __cplusplus

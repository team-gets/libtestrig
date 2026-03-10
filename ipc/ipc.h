#pragma once

// Headers
#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#include <winsock2.h>
#include <afunix.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/unistd.h>
#endif // _WIN32

#include <stdint.h>
#include "message.h"

#ifdef __cplusplus
} // extern "C"
namespace VSCL {
extern "C" {
#endif // __cplusplus

/*
 *	Create a Unix domain socket to a random file.
 *
 *	Returns a nonzero int representing the file descriptor, otherwise -1.
 */
int SockSetup(struct sockaddr_un* sockaddr_mut);

/*
 *	Bind the Unix domain socket, using the path specified in the passed sockaddr_un struct.
 *
 *	Returns 0 on success, -1 on failure.
 */
int SockBind(const int fd, const struct sockaddr_un* sockaddr);

/*
 *	Connect the Domain Socket, using the path specified in the passed sockaddr_un struct.
 *
 *	Returns 0 on success, -1 on failure.
 */
int SockConnect(const int fd, const struct sockaddr_un* sockaddr);

/*
 *	Close the socket.
 */
int SockClose(const int fd, struct sockaddr_un* sockaddr);

/*
 *	Set the socket up to receive and simply write out to the buffer.
 */
int SockReadOut(const int fd, const struct sockaddr_un* sockaddr, uint8_t* buf_out, size_t max_write, int flags);

/*
 *	Set the socket up to receive and loop the handler on each message.
 */
int SockReadAndHandle(const int fd, struct sockaddr_un* sockaddr, int(*handler)(uint8_t*));

/*
 *	Send a message over the socket without waiting for a response.
 */
int SockSend(const int fd, struct RigMessage* msg);

/*
 *
 */
int IdentifyHeaderPart(uint8_t in[4], int idx);

/*
 *
 */
int IdentifyFullHeader(uint8_t in[4]);

#ifdef __cplusplus
} // extern "C"
} // namespace VSCL
#endif

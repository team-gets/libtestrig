#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#inlcude <winsock2.h>
#include <afunix.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/unistd.h>
#endif // _WIN32

#include <stdint.h>

#ifdef __cplusplus
} // extern "C"
namespace Gets {
extern "C" {
#endif // __cplusplus

/*
 *	A test rig message.
 *	These are unsigned chars - but more explict - on the architectures and OSes we care about
 *	(x86_64 Linux and Windows, basically whatever Maxon supports)
 */ 
struct RigMessage {
	// Header: Four bytes since might as well
	uint8_t head[4];

	// Data: Eight bytes as described by the EPOS2 manual's part on CAN frames.
	uint8_t data[8];
};

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
int SockClose(const int fd);

/*
 *	Set the socket up to receive and decode bytestreams.
 */
int SockLoopReceive(const int fd);

/*
 *	Send a message over the socket without waiting for a response (?)
 */
int SockSend(const int fd, struct RigMessage* msg);

#ifdef __cplusplus
} // extern "C"
} // namespace Gets
#endif

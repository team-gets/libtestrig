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

#include "ipc/message.h"
#include "ipc/os.h"
#include "libtestrig_api.h"

/*
 *	Generate a path to a socket file.
 *
 *	This is in a temporary path with the .rigsock extension.
 */
TESTRIG_API int vscl_sock_generate_path(char* sockpath);

/*
 *	Create a Unix socket to a random file.
 *
 *	Returns a nonzero int representing the file descriptor, otherwise -1.
 */
TESTRIG_API int vscl_sock_setup(struct sockaddr_un* sockaddr_mut);

/*
 *	Bind the Unix socket, using the path specified in the passed sockaddr_un struct.
 *
 *	Returns 0 on success, -1 on failure.
 */
TESTRIG_API int vscl_sock_bind(const int fd, const struct sockaddr_un* sockaddr);

/*
 *	Set the Unix socket to listen and permit connection attempts.
 *
 *	Returns 0 on success, -1 on failure.
 */
TESTRIG_API int vscl_sock_listen(const int fd, int max_backlog);

/*
 *	Connect the Unix Socket, using the path specified in the passed sockaddr_un struct.
 *
 *	Returns 0 on success, -1 on failure.
 */
TESTRIG_API int vscl_sock_connect(const int fd, const struct sockaddr_un* sockaddr);

/*
 *	Close the socket and clean up.
 */
TESTRIG_API int vscl_sock_close(const int fd, struct sockaddr_un* sockaddr);

/*
 *	Set the socket up to receive and simply write out to the buffer.
 */
TESTRIG_API int vscl_sock_read_out(const int fd, const struct sockaddr_un* sockaddr, uint8_t* buf_out, size_t max_write, int flags);

/*
 *	Set the socket up to receive and loop the handler on each message.
 */
TESTRIG_API int vscl_sock_read_and_handle(const int fd, struct sockaddr_un* sockaddr, int(*handler)(uint8_t*));

/*
 *	Send a message over the socket without waiting for a response.
 */
TESTRIG_API int vscl_sock_send(const int fd, struct rig_message* msg);

/*
 *	Identify the header byte.
 */
TESTRIG_API int vscl_identify_header_part(uint8_t in[4], int idx);

/*
 *	Identify the four bytes in the header.
 */
TESTRIG_API int vscl_identify_full_header(uint8_t in[4]);

#ifdef __cplusplus
} // extern "C"
#endif

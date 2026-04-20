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

#include <limits.h>
#include <assert.h>

#include "ipc/os.h"
#include "ipc/message.h"
#include "libtestrig_api.h"

/*
 *	@brief Generate a path to a socket file.
 *
 *	This is in a temporary path with the .rigsock extension.
 */
TESTRIG_API int vscl_sock_genpath(char* sockpath);

/*
 *	@brief Create a Unix socket to a random file.
 *
 *	Returns a nonzero int representing the file descriptor, otherwise -1.
 */
TESTRIG_API int vscl_sock_setup(struct sockaddr_un* sockaddr_mut);

/*
 *	@brief Bind the Unix socket, using the path specified in the passed sockaddr_un struct.
 *
 *	Returns 0 on success, -1 on failure.
 */
TESTRIG_API int vscl_sock_bind(const int fd, const struct sockaddr_un* sockaddr);

/*
 *	@brief Set the Unix socket to listen and permit connection attempts.
 *
 *	Returns 0 on success, -1 on failure.
 */
TESTRIG_API int vscl_sock_listen(const int fd, int max_backlog);

/*
 *	@brief Connect the Unix Socket, using the path specified in the passed sockaddr_un struct.
 *
 *	Returns 0 on success, -1 on failure.
 */
TESTRIG_API int vscl_sock_connect(const int fd, const struct sockaddr_un* sockaddr);

/*
 *	@brief Close the socket and clean up.
 */
TESTRIG_API int vscl_sock_close(const int fd, struct sockaddr_un* sockaddr);

/*
 *	@brief Send a message over the socket without waiting for a response.
 *  TODO: make version that waits
 */
TESTRIG_API int vscl_sock_send(const int fd, struct rig_message* msg);

/*
 *	@brief Identify the header byte.
 */
TESTRIG_API int vscl_ident_header_part(vscl_byte_t in[4], int idx);

/*
 *	@brief Identify the four bytes in the header.
 */
TESTRIG_API int vscl_ident_full_header(vscl_byte_t in[4]);

#ifdef __cplusplus
} // extern "C"
#endif
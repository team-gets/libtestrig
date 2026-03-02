#pragma once
#ifdef __cplusplus
namespace Gets {
extern "C" {

#ifdef _WIN32
#inlcude <winsock2.h>
#include <afunix.h>
#else
#include <sys/socket.h>
#include <sys/un.h>
#endif // _WIN32
#endif // __cplusplus

#include <stdint.h>

struct RigMessage {
	uint8_t head[4];
	uint8_t data[8]; // guessed from EPOS2 docs, guaranteed 8 bytes, no guarantee on bits, technically
};

int SockSetup();
int SockSend(char* msg);
int SockClose();

#ifdef __cplusplus
} // extern "C"
} // namespace Gets
#endif

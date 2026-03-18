#define _UNICODE
#define UNICODE

#include <stdio.h>

#include "ipc/ipc.h"
#include "ipc/constants.h"

#include <windows.h>

int main(int argc, char** argv) {
    if (argc < 2) { printf("Usage: %s <socket path>", argv[0]); return -1; }

    int retstat;
    int sock;
    struct RigMessage msg;
    uint8_t data[8] = { 'H', 'O', 'W', 'D', 'Y', '!', '!', '!'};
    SetMessage(&msg, HEAD_DC, data);

    struct sockaddr_un sockaddr;
    strncpy(sockaddr.sun_path, argv[1], strlen(argv[1]) + 1);

    sock = SockSetup(&sockaddr);
    if (sock == INVALID_SOCKET) { perror("Client socket is invalid"); return 1; }

    retstat = SockConnect(sock, &sockaddr);
    if (retstat == SOCKET_ERROR) { perror("Client socket failed to connect"); return 1; }

    retstat = SockSend(sock, &msg);
    if (retstat == SOCKET_ERROR) { perror("Client socket failed to send"); return 1; }

	retstat = SockClose(sock, &sockaddr);
	if (retstat == -1) { perror("Client socket cleanup error"); return 1; }

    return 0;
}
#define _UNICODE
#define UNICODE

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "ipc/ipc.h"
#include "ipc/constants.h"

#include <windows.h>

int write_out_sockname(const char* sockname) {
    // not gonna open a pipe bro
    WCHAR cwd[128] = { 0 };
    WCHAR txt[168] = { 0 };

    GetCurrentDirectory(128, cwd);
    wsprintf(txt, L"%ls\\sock.txt", cwd);

    FILE* where2 = _wfopen(txt, L"w");
    fwrite(sockname, 1, strlen(sockname), where2);
    fclose(where2);

    printf("Wrote socket name to %ls\n", txt);

    return 0;
}

int main(int argc, char** argv) {
    // locals
    int retstat;
    int bytes_recv;
    int sock;
    struct sockaddr_un sockaddr;
    memset(&sockaddr, 0, sizeof(struct sockaddr_un));

    char sockpath[108] = { 0 };

    // Grab preset socket path, if any
    if (argc >= 2) {
        strncpy(sockpath, argv[1], 108);
        printf("Server will create socket at %s\n", sockpath);
    }

    // Setup
    sock = SockSetup(&sockaddr);
    if (sock == INVALID_SOCKET) { perror("Server socket is invalid"); return 1; }
    
    printf("Making a sockname file using socket path %s...\n", sockaddr.sun_path);
    write_out_sockname(sockaddr.sun_path);
    
    retstat = SockBind(sock, &sockaddr);
    if (retstat == SOCKET_ERROR) { perror("Server socket failed to bind"); return 1; }

    retstat = SockListen(sock, 2);
    if (retstat == -1) { perror("Server socket failed to start listening"); return 1; }

    uint8_t buf[1024] = { 0 };
    bytes_recv = SockReadOut(sock, &sockaddr, buf, 1024, DC_WITH_CLIENT);

    printf("Server is cleaning up after receiving %i bytes...\n", bytes_recv);

	retstat = SockClose(sock, &sockaddr);
	if (retstat == -1) { perror("Server socket cleanup error"); return 1; }
	printf("Server received %s\n", buf);

    return 0;
}
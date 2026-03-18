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
    HANDLE fout;
    WCHAR widesock_temp[108] = { 0 };
    WCHAR widesock[108] = { 0 };
    WCHAR cwd[128] = { 0 };
    WCHAR where2[128] = { 0 };
    char comeback[128] = { 0 };

    mbstowcs(widesock_temp, sockname, 108);
    mbstowcs(widesock, sockname, 108);
    GetCurrentDirectory(128, cwd);

    int occurence = 0;
    WCHAR* buf;
    WCHAR* actual_sockname = wcstok(widesock_temp, L"\\", &buf);
    while (actual_sockname) {
        actual_sockname = wcstok(NULL, L"\\", &buf);
        occurence++;
    }

    WCHAR* buf2;
    int n = 0;
    actual_sockname = wcstok(widesock, L"\\", &buf2);
    while (actual_sockname && n < occurence - 1) {
        actual_sockname = wcstok(NULL, L"\\", &buf2);
        n++;
    }

    wsprintf(where2, L"%ls\\%ls", cwd, actual_sockname);
    printf("I will place it at %ls\n", where2);
    wcstombs(comeback, where2, 128);

    fout = CreateFileA(
        comeback,
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    CloseHandle(fout);
    return 0;
}

int main(int argc, char** argv) {
    // locals
    int retstat;
    int bytes_recv;
    int sock;
    struct sockaddr_un sockaddr;
    char sockpath[108] = { 0 };

    // Grab preset socket path, if any
    if (argc >= 2) {
        strncpy(sockpath, argv[1], 108);
        printf("Server will create socket at %s\n", sockpath);
    }

    // Setup
    sock = SockSetup(&sockaddr);
    if (sock == INVALID_SOCKET) { perror("Server socket is invalid"); return 1; }
    
    printf("Making a sockname file...\n");
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
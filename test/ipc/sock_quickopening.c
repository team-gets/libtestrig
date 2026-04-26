#include "ipc/sock.h"
#include "ipc/os.h"
#include <stdio.h>


int main(int argc, char** argv) {
    struct sockaddr_un sockaddr = { 0 };
    int sock = vscl_sock_setup(&sockaddr);
    if (sock == INVALID_SOCKET) { vscl_os_perror("Socket quickopen fail: setup"); return -1; }
    else { printf("Sock created for quickopen at %s\n", sockaddr.sun_path); }

    int bound = vscl_sock_bind(sock, &sockaddr);
    if (bound == -1) { vscl_os_perror("Socket quickopen fail: bind"); return -1; }

    printf("Goodbye!\n");
    return vscl_sock_close(sock, &sockaddr);
}
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "ipc/sock.h"
#include "ipc/os.h"

int main(void) {
    printf("Begin socket naming tests.\n");
    int retstat = 0;

    char sockdest[108] = { 0 };
    retstat = vscl_get_sock_destination(sockdest);
    if (retstat != 0) { fprintf(stderr, "Failed to find socket path: sockpath became %s\n", sockdest); return retstat; }

    printf("Sockets will go to %s\n", sockdest);
    memset(sockdest, 0, 1);
    retstat = vscl_sock_genpath(sockdest);
    if (retstat != 0) { fprintf(stderr, "Could not generate a proper path for the socket.\n"); return retstat; }

    char new_sockdest[108] = { 0 };
    retstat = vscl_sock_genpath(new_sockdest);
    if (retstat != 0) { fprintf(stderr, "Could not generate a proper path for the fresh socket.\n"); return retstat; }

    printf("The socket path was randomly:       %s\n", sockdest);
    printf("The fresh socket path was randomly: %s\n", new_sockdest);

    // Ensure no early truncation
    assert(strstr(sockdest, ".rigsock") != NULL);
    assert(strstr(new_sockdest, ".rigsock") != NULL);
    printf("The socket path was not truncated.\n");

    return 0;
}
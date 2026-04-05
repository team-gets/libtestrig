/*
 *  Unit test to validate Socket paths by generating three of them.
 *  Due to the speed of these tests, the RNG seed does not change, so they are the same.
 *  This behavior is not guaranteed normally, so it is not tested for.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ipc/ipc.h"

void check_ret(int ret) {
    if (ret != 0) {
        perror("sock path generation");
        exit(ret);
    }
}

int main(void) {
    char sock1[108] = { 0 };
    char sock2[108] = { 0 };
    char sock3[108] = { 0 };
    int ret;

    ret = SockGeneratePath(sock1);
    check_ret(ret);
    printf("Generated sock path:\n%s\n", sock1);

    ret = SockGeneratePath(sock2);
    check_ret(ret);
    printf("Generated sock path:\n%s\n", sock2);

    ret = SockGeneratePath(sock3);
    check_ret(ret);
    printf("Generated sock path:\n%s\n", sock3);

    return ret;
}

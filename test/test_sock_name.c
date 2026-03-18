#include <stdio.h>

#include "ipc/ipc.h"
#include "ipc/constants.h"

void check_ret(int ret) {
    if (ret != 0) {
        perror("sock path generation");
        exit(ret);
    }
}

int main(int argc, char** argv) {
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
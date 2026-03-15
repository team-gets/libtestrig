#define _UNICODE
#define UNICODE
// Windows tests will operate with wide chars
// except for the entry point?? LOL!!!

#include <stdio.h>

#include "ipc/ipc.h"
#include <windows.h>

int main(int argc, char** argv) {
    if (argc < 2) { printf("Usage: %s <socket path>", argv[0]); return -1; }

}
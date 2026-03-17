#define _UNICODE
#define UNICODE

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int main(int argc, char** argv) {
    STARTUPINFO siserve;
    STARTUPINFO siclient;

    PROCESS_INFORMATION piserve;
    PROCESS_INFORMATION piclient;

    memset(&siserve, 0, sizeof(STARTUPINFO));
    memset(&siclient, 0, sizeof(STARTUPINFO));
    memset(&piclient, 0, sizeof(PROCESS_INFORMATION));
    memset(&piclient, 0, sizeof(PROCESS_INFORMATION));

    siserve.cb = sizeof(siserve);
    siclient.cb = sizeof(siclient);

    // Start the child process. 
    if(!CreateProcess(NULL,
        L"./test_server.exe",
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL, 
        &siserve,
        &piserve)
    ) 
    {
        perror("create serve");
        return -1;
    }

    WaitForSingleObject(piserve.hProcess, INFINITE);

    CloseHandle(piserve.hProcess);
    CloseHandle(piserve.hThread);

    return 0;
}
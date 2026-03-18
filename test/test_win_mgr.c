#define _UNICODE
#define UNICODE

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int main(int argc, char** argv) {
    printf("Manager starting\n");

    STARTUPINFO siserve;
    STARTUPINFO siclient;

    PROCESS_INFORMATION piserve;
    PROCESS_INFORMATION piclient;

    printf("Manager setting\n");
    memset(&siserve, 0, sizeof(STARTUPINFO));
    memset(&siclient, 0, sizeof(STARTUPINFO));
    memset(&piclient, 0, sizeof(PROCESS_INFORMATION));
    memset(&piclient, 0, sizeof(PROCESS_INFORMATION));

    siserve.cb = sizeof(siserve);
    siclient.cb = sizeof(siclient);

    WCHAR cwd[128];
    WCHAR cmd[256];
    WCHAR cmd2[512];
    WCHAR cwdwild[129];
    GetCurrentDirectory(128, cwd);

    wsprintf(cmd, L"%ls\\%ls", cwd, L"test_server.exe");
    wsprintf(cwdwild, L"%ls\\%ls", cwd, L"*");

    printf("Manager starting server with %ls\n", cmd);
    BOOL servemk = CreateProcess(NULL,
        cmd,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL, 
        &siserve,
        &piserve);
    if (servemk == 0) {
        printf("Manager failed to create server: %lu\n", GetLastError());
        return -1;
    }
    

    WIN32_FIND_DATA wddata;
    WCHAR fname[256] = { 0 };
    wsprintf(fname, L"%ls\\", cwd);
    HANDLE hfind = FindFirstFile(cwdwild, &wddata);

    if (hfind != INVALID_HANDLE_VALUE) {
        while (FindNextFile(hfind, &wddata)) {
            WCHAR* wideguy = wcsstr(wddata.cFileName, L".sock");
            if (wideguy != NULL) {
                wcscat_s(fname, 256, wddata.cFileName);
                break;
            }
        }

        FindClose(hfind);
    }

    Sleep(1000);
    wsprintf(cmd2, L"%ls\\%ls %ls", cwd, L"test_client.exe", fname);
    printf("Manager starting client with %ls\n", cmd2);
    BOOL clientmk = CreateProcess(NULL,
        cmd2,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL, 
        &siclient,
        &piclient);
    if (clientmk == 0) {
        printf("Manager failed to create client: %lu\n", GetLastError());
        return -1;
    }

    printf("Waiting for the server and client to exit...\n");
    WaitForSingleObject(piserve.hProcess, INFINITE);

    CloseHandle(piserve.hProcess);
    CloseHandle(piserve.hThread);

    printf("Manager exiting\n");
    return 0;
}
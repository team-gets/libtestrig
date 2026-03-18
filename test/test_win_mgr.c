#define _UNICODE
#define UNICODE

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

int main(int argc, char** argv) {
    if (argc < 3) { printf("Improper invocation\n"); return -1; }
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

    WCHAR cwd[128] = { 0 };
    WCHAR cmd1[256] = { 0 };
    WCHAR cmd2[512] = { 0 };

    GetCurrentDirectory(128, cwd);
    mbstowcs(cmd1, argv[1], 256);
    mbstowcs(cmd2, argv[2], 256);

    printf("Manager starting server with %ls\n", cmd1);
    BOOL servemk = CreateProcess(NULL,
        cmd1,
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
    WCHAR cwdwild[130] = { 0 };
    WCHAR fname[256] = { 0 };
    wsprintf(fname, L"%ls\\", cwd);
    wsprintf(cwdwild, L"%ls\\*", cwd);
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
    wcscat(cmd2, L" ");
    wcscat(cmd2, fname);
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
    WaitForSingleObject(piclient.hProcess, INFINITE);

    CloseHandle(piserve.hProcess);
    CloseHandle(piserve.hThread);
    CloseHandle(piclient.hProcess);
    CloseHandle(piclient.hThread);

    printf("Manager exiting\n");
    return 0;
}
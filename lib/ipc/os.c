#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "os.h"

int vscl_get_sock_destination(char *dest) {
#ifdef _WIN32
    size_t retvalue;
    char usrtemp[76] = { 0 };
    errno_t retstat;
    
    retstat = getenv_s(&retvalue, usrtemp, 76, "TEMP");
    if (retvalue == 0 || retstat != 0) { perror("Failed to get TEMP"); return retstat; }
    
	strncpy(dest, usrtemp, retvalue);
    strncat(dest, "\\", 2);
    return 0;

#else
	strncpy(dest, "/tmp/", 6);
    return 0;
#endif
}

#ifdef _WIN32
void vscl_winprint_error(const TCHAR* msg) {
    DWORD errcode = GetLastError();
    TCHAR errmsg[256] = { 0 };

    DWORD wides = FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errcode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errmsg,
            256,
            NULL);


    if (wides == 0) { printf("error while processing error\n"); }
    wprintf(L"%s: %s", msg, errmsg);
}
#endif
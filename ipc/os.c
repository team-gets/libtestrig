#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "os.h"

int GetSockDestination(char *dest) {
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "os.h"

[[maybe_unused]]
static size_t vscl_strinterlen(const char* interstring, const char* original) {
	assert(original >= interstring);
	return original - interstring;
}

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

int vscl_make_new_proc(const char* prog, const char* args) {
#if _WIN32
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };

	char cmd[1024] = { 0 };
	sprintf(cmd, "%s %s", prog, args);

	BOOL mkdetach = CreateProcess(
			NULL,
			cmd,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi
		);

	if (mkdetach == FALSE) { vscl_winprint_error("failed to detach"); return -1; }

	CloseHandle(&si);
	CloseHandle(&pi);

	return GetCurrentProcessId();

#else
	pid_t pid = fork();

	switch (pid) {
	case -1:
		perror("failed to fork off");
		return -1;
		break;
	case 0:
		if (setsid() == -1) {
			perror("failed to detach");
			return -1;
		}
		else {
			char** argv = (char**)calloc(2, sizeof(char*));
			size_t num_args = 2;
			size_t sizeof_argv = 2;

			argv[0] = (char*)malloc(strlen(prog) + 1);
			strncpy(argv[0], prog, strlen(prog) + 1);

			argv[1] = (char*)calloc(64, 1);
			char* arg1 = strstr(args, " ");
			if (arg1 == NULL) {
				strncpy(argv[1], args, strlen(args) + 1);
			}
			else {
				char interstr[64] = { 0 };
				size_t interlen = vscl_strinterlen(args, arg1);

				strncpy(interstr, args, interlen);
				strncpy(argv[1], interstr, strlen(interstr) + 1);

				char* argn = strstr(arg1, " ");
				char* argn1 = arg1;
				while (argn != NULL) {
					num_args++;
					if (num_args > sizeof_argv) {
						sizeof_argv *= 2;
						argv = (char**)realloc(argv, sizeof_argv * sizeof(char*));
					}

					size_t n = num_args - 1;
					argv[n] = (char*)calloc(64, 1);
					char argnstr[64] = { 0 };
					size_t argnlen = vscl_strinterlen(argn, argn1);
					argnlen = (argnlen == 0) ? 64 : argnlen;

					strncpy(argnstr, argn1, argnlen);
					strncpy(argv[n], argnstr, strlen(argnstr) + 1);

					argn1 = argn;
					argn = strstr(argn1 + 1, " ");
				}
			}

			execv(prog, argv);

			for (size_t i = 0; i < num_args; i++) {
				free(argv[i]);
			}

			free(argv);
			return 0;
		}
		break;
	default:
		return pid;
		break;
	}
#endif
}

void vscl_sleep(uint32_t s) {
#ifdef _WIN32
	Sleep(s * 1000);
#else
	sleep(s);
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

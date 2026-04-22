#include "ipc/sock.h"
#include "ipc/os.h"
#include <stdio.h>

int main(int argc, char** argv) {
    printf("This was invoked with the following command line:\n");
    printf("\t%s %s\n", argv[0], argv[1]);

    if (strncmp(argv[1], "serve", 6) == 0) {
        int retstat = vscl_make_new_proc(argv[0], "child");
        if (retstat == -1) { fprintf(stderr, "Failure to launch\n"); return retstat; }
        else { printf("New process was spawned with PID %i\n", retstat); }
    }

    return 0;
}
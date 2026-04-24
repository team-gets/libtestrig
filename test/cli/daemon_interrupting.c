#include <stdio.h>
#include <string.h>

#include "ipc/os.h"

int main(int argc, char** argv) {
    if (argc < 2) { fprintf(stderr, "This test requires arguments.\n"); return -1; }

    int made = vscl_make_new_proc(argv[1], "daemon");
    vscl_sleep(2);
    int made2 = vscl_make_new_proc(argv[1], "daemon down");
    vscl_sleep(1);
    
    char sockpath[108] = { 0 };
    int making_path = vscl_get_sock_destination(sockpath);
    if (making_path) { fprintf(stderr, "Failed to get sock destination. Did an upstream test fail?\n"); return -1; }

    strncat(sockpath, "testrigd.sock", 15);

    FILE* potential_sock = fopen(sockpath, "r");
    int not_there = potential_sock == NULL;

    return (made != -1 && made2 != -1 && not_there) ? 0 : -1;
}

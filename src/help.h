#pragma once

#include <stdint.h>
#include "other_args.h"

int help_me(other_args* which);
void print_usage(void);

void printhelp_help(void);
void printhelp_ident(void);
void printhelp_stat(void);
void printhelp_daemon(void);
void printhelp_open(void);
void printhelp_record(void);
void printhelp_close(void);

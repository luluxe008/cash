#ifndef __EXE_H__
#define __EXE_H__
#include "parser.h"
#include <stdbool.h>

typedef struct ExitCode{
    int exit_code;
    int exit_signal;
} ExitCode;

/*Execute the program. If cannot execute the program, return NULL*/
ExitCode* execute_command(Command*);


#endif
#ifndef __EXE_H__
#define __EXE_H__
#include "parser.h"
#include <stdbool.h>
/*
static const char* PATH[] = { //hardcoded
    "/usr/bin/",
    "/usr/sbin/",
    "/usr/local/bin",
    "/usr/local/sbin",
    "/bin/",
    "/sbin/", 
};// TODO, make them read PATH*/

typedef enum ResultType{ //FIXME unused
    ExitCode,
    ExitSignal,
    CoreDumped,
    Output,
    ErrOutput,
} ResultType;

typedef struct CommandResult{
    enum ResultType type;
    int exit_code;
    int term_signal;
    bool core_dumped;
    char* output;
    char* err;
    bool executed;
} CommandResult;

CommandResult execute_command(Command* command, bool save_stdout, bool save_stderr );


#endif
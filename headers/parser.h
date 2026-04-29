#ifndef __PARSER_H__
#define __PARSER_H__

#define MAX_COMMAND_SIZE 1024 //to change?
#define MAX_ARGC 100
#define MAX_ARG_SIZE 1024
#include <stdio.h>

// first argument is the command
typedef struct Command{
    struct Argument* args;
    int argc;
} Command;

typedef struct Argument{
    char* litteral;
    Command* cmd;
} Argument;

Command make_command(const char* string);
void free_command(Command* cmd);


#endif
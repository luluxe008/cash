#ifndef __PARSER_H__
#define __PARSER_H__

#define MAX_COMMAND_SIZE 1024 //to change?
#define MAX_ARGC 100
#define MAX_ARG_SIZE 256

typedef struct Command{
    char* program_name;
    int argc;
    char** args;
} Command;

void free_command(Command* command);
/*
    Parse a null-terminated string into a Command
    The string must not contain a newline.

*/
Command parse_command(const char* cmd);

#endif
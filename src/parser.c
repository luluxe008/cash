
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"

void free_command(Command* command){
    free(command->program);
    for (int i=0; i<command->argc; i++){
        free(command->args[i]);
    }
    free(command->args);
}


Command parse_command(const char* cmd){
    /*
    First step: find the program
    Second step: find its argument and their number
    argument are seprated by spaces
    
    */


    size_t len = strlen(cmd); 
    char tmp_command[MAX_COMMAND_SIZE]= {0};
    size_t current_char = 0;
    
    for (current_char=0; current_char<len && cmd[current_char]!=' '; current_char++){
        tmp_command[current_char] =cmd[current_char];
    }

    char* command = malloc(strlen(tmp_command)*sizeof(char)+1);
    strcpy(command, tmp_command);

    int argc = 0;

    char* tmp_args[MAX_ARGC];

    int arg_index = 0;

    for (; current_char<len; current_char++){
        if (cmd[current_char] == ' '){
            argc+=1;
            tmp_args[argc-1] = malloc(MAX_ARG_SIZE*sizeof(char));
            memset(tmp_args[argc-1], 0, MAX_ARG_SIZE);
            arg_index = 0;
            continue;
        }
        printf("%c", cmd[current_char]);
        tmp_args[argc-1][arg_index] = cmd[current_char]; //argc must be different than 0 at this point, because after command there is a space
        arg_index+=1;

    }

 

    char** argv = malloc(argc*sizeof(char*));
    for (int i=0; i<argc; i++){
        argv[i] = malloc(strlen(tmp_args[i])*sizeof(char)+1);
        strcpy(argv[i], tmp_args[i]);
    }

    Command res = {
        command,
        argc,
        argv,
    };
    for (int i=0; i<argc; i++){
        free(tmp_args[i]);
    }
    return res;
}

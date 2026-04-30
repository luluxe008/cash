#include <stdio.h>
#include "debug.h"


void _print_level(int level){
    for (int i=0; i<level; i++) printf("\t");
    printf("|");

}

void _debug_command(const Command* command, int level){
   

    for (int i=0; i<command->argc; i++){
        if (command->args[i].litteral){
            _print_level(level);
            printf("%d:\"%s\"\n", i, command->args[i].litteral);
        }
        else if (command->args[i].cmd){
            _print_level(level);
            printf("%d:cmd:\n", i);
            _debug_command(command->args[i].cmd, level+1);
        }
        
        
    }
}


void debug_command(const Command* command){
    _debug_command(command, 0);
}

void debug_arg(const Argument* arg){
    if (arg->litteral){
        printf("arg is string: %s\n", arg->litteral);
    }else{
        printf("arg is cmd: %p\n", (void*)arg->cmd);
    }
}

void debug_command_result(const CommandResult* res){
    if (!res->executed){
        printf("comand not executed");
        return;
    }
    printf("exit code: %d\n", res->exit_code);
    printf("term signal: %d\n", res->term_signal);
    printf("core dumped: %s\n", res->core_dumped ? "true" : "false");
}

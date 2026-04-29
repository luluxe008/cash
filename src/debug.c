#include <stdio.h>
#include "debug.h"

void _print_level(int level){
    for (int i=0; i<level; i++) printf("\t");
    printf("|");

}

void _debug_command(Command* command, int level){
   

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


void debug_command(Command* command){
    _debug_command(command, 0);
}

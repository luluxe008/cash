#include <stdio.h>
#include "debug.h"

void debug_command(Command* command){
    printf("cmd: %s\n", command->program_name);
    printf("argc: %d\n", command->argc);
    
    for (int i=0; i<command->argc; i++){
        printf("%d: %s\n", i+1, command->args[i]);
    }

}
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> //getcwd chdir execv fork pipe
//https://www.geeksforgeeks.org/c/making-linux-shell-c/

#include "parser.h"
#include "exe.h"
#include "debug.h"


void quick_quit(const char* buf){
    if (strcmp(buf, "q") == 0 || strcmp(buf, "quit") == 0 || strcmp(buf, "exit") == 0 ){
        printf("quick quit\n");
        exit(EXIT_SUCCESS);
    }
}

void remove_newline(char* buf){
    for (int i=0; buf[i]!='\0'; i++) {
        if (buf[i] == '\n') buf[i] = '\0';
    }
}

bool skip_empty(char* buf){
    return strlen(buf) == 0 ;
}

int main(void){
    char cmd_buf[1024] = {0};
    while (true){
        printf("> ");
        fgets(cmd_buf, MAX_COMMAND_SIZE, stdin);
        remove_newline(cmd_buf);
        if (skip_empty(cmd_buf)) continue;
        
        quick_quit(cmd_buf);
        
        Command res = parse_command(cmd_buf);
        execute_command(&res);
        
        //debug_command(&res);
        free_command(&res);
    }
    return 0;
}
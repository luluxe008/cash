#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> //getcwd chdir execv fork pipe
#include <ctype.h>

//https://www.geeksforgeeks.org/c/making-linux-shell-c/

#include "parser.h"
//#include "exe.h"
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

char* trim_trailing_spaces(char* buf){
    size_t len = strlen(buf);
    if (isspace(buf[len-1])){
        buf[len-1] = '\0';
        return trim_trailing_spaces(buf);
    }
    return buf;
}

bool skip_empty(char* buf){
    return strlen(buf) == 0 ;
}

int main(void){
    char cmd_buf[1024] = {0};
    Command command = {0};
    while (true){
        printf("> ");
        fgets(cmd_buf, MAX_COMMAND_SIZE, stdin);
        remove_newline(cmd_buf);
        if (skip_empty(cmd_buf)) continue;
        trim_trailing_spaces(cmd_buf);
        printf("strlen(\"%s\")=%d\n", cmd_buf, strlen(cmd_buf));
        quick_quit(cmd_buf);

        command = make_command(cmd_buf);
        debug_command(&command);
        /*
        Command res = parse_command(cmd_buf);
        execute_command(&res);
        */
        //debug_command(&res);
        //free_command(&res);
    }
    return 0;
}
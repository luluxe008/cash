#include "exe.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // for some reason VS Code cries if this is not here
#include <sys/wait.h>
#include <errno.h>

#include "debug.h"

bool can_acces_program(const char* program_path){
    return access(program_path, R_OK | X_OK) == 0; // file can be read and executed
}


char* make_program_path(const char* base_path, const char* program_name){
    char* program_path = malloc((strlen(base_path)+strlen(program_name)+1)*sizeof(char));
    memset(program_path, 0, (strlen(base_path)+strlen(program_name)+1)); // in some case we were writing on garbage
    strcat(program_path, base_path);
    strcat(program_path, program_name);
    return program_path;
}

char* _resolve_argument(Argument* arg){
    /* Two cases:
        1. arg is just a litteral
            => we return it
        2. arg is cmd
            => we execute_command and return the appropriate result
    */
    if (arg->litteral){
        
        return arg->litteral;
    }
    else{
        //for now, only exit code
        CommandResult res = execute_command(arg->cmd);
        char* s = malloc(25); // 25 may be enough
        sprintf(s, "%d", res.exit_code);
        return s;
    }
}

CommandResult execute_command(Command* command){
    /*Steps:
        First we check for flags:
            - "c" for exit code
            - "s" for exit signal
            - "o" for stdout-output
            - "e" for stderr-output
            - "d" to check if core dumped
            - None, which will be equivalent to c
        Second, we locate the binary
        Third we fork and execv
    */

    //we skip flag check for now


     CommandResult res = { // this is garbage
            .type = 0,
            .exit_code = -1,
            .term_signal = -1,
            .core_dumped = false,
            .output = false,
            .err = false
    };
   
    char* program_name = _resolve_argument(&command->args[0]);

    char* program_path = make_program_path("/usr/bin/", program_name);

    printf("program name is : \"%s\"\n", program_name);
    printf("program path is : %s\n", program_path);

    if (!can_acces_program(program_path)){
        printf("cash: Unable to open \"%s\": %s\n", program_name, strerror(errno));
        return res;
    }


    pid_t pid = fork();
    if (pid < 0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0){
        //child
        char **argv = malloc((command->argc+2)*sizeof(char*)); //FIXME, if a subcommand expands to more, there are more argc
        // +2 for program_name and final NULL
        argv[0] = program_name;
        argv[command->argc+1] = NULL;
        for (int i=1; i<command->argc; i++){
            char* arg = _resolve_argument(&command->args[i]);

            argv[i] = calloc(strlen(arg)+1, sizeof(char));
            strcpy(argv[i], arg);
        }


        execv(program_path, argv);
    }
    else{
        //parent
        int wstatus;
        wait(&wstatus);


        printf("process over\n");
        
        if (WIFEXITED(wstatus)){
            res.exit_code = WEXITSTATUS(wstatus);
        }
        if (WIFSIGNALED(wstatus)){
            res.exit_code = WTERMSIG(wstatus);
        }
        if (WCOREDUMP(wstatus)){
            res.core_dumped = true;
        }
        
        free(program_path);
        
        return res;
    }

    return res; //unreachable
}

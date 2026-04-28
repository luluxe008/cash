#include "exe.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // for some reason VS Code cries if this is not here
#include <sys/wait.h>

bool can_acces_program(const char* program_path){
    return access(program_path, R_OK | X_OK) == 0; // file can be read and executed
}


char* make_program_path(const char* base_path, const char* program_name){
    char* program_path = malloc((strlen(base_path)+strlen(program_name)+1)*sizeof(char));
    strcat(program_path, base_path);
    strcat(program_path, program_name);
    return program_path;
}

ExitCode* execute_command(Command* cmd){
    //for now, only /usr/bin/
    printf("executing %s\n", cmd->program_name);
    // first we prepare the program_name
    char* program_path = make_program_path("/usr/bin/", cmd->program_name);
    /*malloc((strlen("/usr/bin/")+strlen(cmd->program_name)+1)*sizeof(char));
    strcat(program_path, "/usr/bin/");
    strcat(program_path, cmd->program_name);*/

    printf("program path: %s\n", program_path);


    // we check if it exists
    if (!can_acces_program(program_path)){
        printf("program don't exist\n");
        return NULL;
    }



    // we prepare the arguments
    char** arguments = calloc(cmd->argc+2, sizeof(char*));
    for (int i=0; i<cmd->argc; i++){
        arguments[i+1] = malloc(strlen(cmd->args[i])+1);
        strcpy(arguments[i+1], cmd->args[i]);
    }
    arguments[0] = malloc(strlen(cmd->program_name)+1);
    strcpy(arguments[0], cmd->program_name);
    arguments[cmd->argc+1] = NULL;


    // we actually execute the program and store its status
    int wstatus;

    pid_t fork_pid = fork();

    if (fork_pid < 0){
        // fail
        perror("fork failed");
        return NULL;
    } 

    else if (fork_pid == 0){
        //child
        int res = execv(program_path, arguments);
        if (res){
            perror("[IMPOSSIBLE] failed to execv");
        }
        return NULL;        
    }

    //parent
    wait(&wstatus);

    ExitCode *res = malloc(sizeof(ExitCode));
    res->exit_code = -1;
    res->exit_signal = -1;


    if (WIFEXITED(wstatus)){
        printf("process exited normally with exit code %d\n", WEXITSTATUS(wstatus));
        res->exit_code = WEXITSTATUS(wstatus);
    }
    if (WIFSIGNALED(wstatus)){
        printf("process was terminated with signal %d\n", WTERMSIG(wstatus));
        res->exit_signal = WTERMSIG(wstatus);
    }
    if  (WCOREDUMP(wstatus)){
        printf("process produced a core dump\n");
    }
    return res;
}

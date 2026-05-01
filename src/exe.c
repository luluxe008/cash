#include "exe.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // for some reason VS Code cries if this is not here
#include <sys/wait.h>
#include <errno.h>

#include <limits.h>

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

//don't forget to free it after use
Command* _make_command_without_flag(Command* src){
    Command* cmd = malloc(sizeof(Command));
    // i think i will copy the whole memory
    cmd->argc = src->argc-1;
    cmd->args = calloc(cmd->argc, sizeof(Argument));
    
    memcpy(cmd->args, &src->args[1],  sizeof(Argument)*cmd->argc);

    return cmd;
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
        // First we check for flags:
        //     - "c" for exit code
        //     - "s" for exit signal
        //     - "o" for stdout-output
        //     - "e" for stderr-output
        //     - "d" to check if core dumped
        //     - None, which will be equivalent to c
        Command* cmd;
        char* final;

        if (arg->cmd->argc > 1 &&  // the subcommand has more than one arg
            arg->cmd->args[0].litteral && //the first subcommand arg is a litteral
            strlen(arg->cmd->args[0].litteral) == 1 // its size is one
        )
        {
            char flag = arg->cmd->args[0].litteral[0];
            cmd = _make_command_without_flag(arg->cmd);
            bool save_stdout = false;
            bool save_stderr = false;

            if (flag == 'e') save_stderr = true;
            if (flag == 'o') save_stdout = true;

            CommandResult res = execute_command(cmd, save_stdout, save_stderr);

            switch (flag)
            {
            case 'c':
                final = malloc(25); // should be enough
                sprintf(final, "%d", res.exit_code);
                break;
            case 's':
                final = malloc(25); // should be enough
                sprintf(final, "%d", res.term_signal);
                break;
            case 'o':
                final = res.output;
                break;

            case 'e':
                //printf("in stderr: %s\n", res.err);
                final = res.err;
                break;

            case 'd':
                if (res.core_dumped){
                    final = malloc(5);
                    strcpy(final, "true");
                }else{
                    final = malloc(6);
                    strcpy(final, "false");
                }
                break;
            default:
                printf("Unknown flag %c, continuing...\n", flag); //put warning here
                break;
            }
        }else{
            cmd = arg->cmd;
            CommandResult res = execute_command(cmd, false, false);
            final = malloc(25); // should be enough
            sprintf(final, "%d", res.exit_code);
        }
        return final;
    }
}

CommandResult execute_command(Command* command, bool save_stdout, bool save_stderr ){
    /*Steps:
        
        Second, we locate the binary
        Third we fork and execv
    */

    //we skip flag check for now
    char* program_name = _resolve_argument(&command->args[0]);


    

    CommandResult res = { // this is garbage
            .type = 0,
            .exit_code = -1,
            .term_signal = -1,
            .core_dumped = false,
            .output = false,
            .err = false,
            .executed = false,
    };
   

    char* program_path = make_program_path("/usr/bin/", program_name);

    if (!can_acces_program(program_path)){
        printf("cash: Unable to open \"%s\": %s\n", program_name, strerror(errno));
        free(program_path);
        return res;
    }


    int pipe_fd[2];

    if (save_stdout || save_stderr){
        if (pipe(pipe_fd) == -1){
            perror("pipe init");
            exit(EXIT_FAILURE);
        }
    }

    pid_t pid = fork();
    if (pid < 0){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0){
        //child
        char **argv = malloc((command->argc+2)*sizeof(char*)); 
        // +2 for program_name and final NULL
        memset(argv, 0, (command->argc+2)*sizeof(char*));

        argv[0] = program_name;
        argv[command->argc+1] = NULL;
        for (int i=1; i<command->argc; i++){
            char* arg = _resolve_argument(&command->args[i]);

            argv[i] = calloc(strlen(arg)+1, sizeof(char));
            strcpy(argv[i], arg);

        }

        
        if (save_stdout){
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[0]);
            //close(pipe_fd[1]);

        }
        if (save_stderr){
            dup2(pipe_fd[1], STDERR_FILENO);
            close(pipe_fd[0]);
            //close(pipe_fd[1]);
        }

        execv(program_path, argv);
    }
    else{
        //parent
        int wstatus;

        char* stdout_buf = NULL;
        char* stderr_buf = NULL;


        if (save_stdout){
            close(pipe_fd[1]);

            char buf[1024];
            int nbytes = read(pipe_fd[0], buf, sizeof(buf));
            stdout_buf = malloc(nbytes+1);
            memcpy(stdout_buf, buf, nbytes);
            stdout_buf[nbytes] = '\0';       
            
            
        }
        if (save_stderr){
            close(pipe_fd[1]);

            char buf[1024];
            int nbytes = read(pipe_fd[0], buf, sizeof(buf));
            stderr_buf = malloc(nbytes+1);
            memcpy(stderr_buf, buf, nbytes);
            stderr_buf[nbytes] = '\0';
            //printf("in stderr, we find: %s\n", stderr_buf);       
        }

        wait(&wstatus);
        //printf("process over\n");
        
        

        res.executed = true;
        if (WIFEXITED(wstatus)){
            res.exit_code = WEXITSTATUS(wstatus);
        }
        if (WIFSIGNALED(wstatus)){
            res.exit_code = WTERMSIG(wstatus);
        }
        if (WCOREDUMP(wstatus)){
            res.core_dumped = true;
        }

        res.output  =   stdout_buf;
        
        res.err     =   stderr_buf;


        free(program_path);
        
        return res;
    }

    //unreachable 
    // branch 1: return with executed set to false
    // branch 2: execv
    // branch 3: return
    return res; 
}

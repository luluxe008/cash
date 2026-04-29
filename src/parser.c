
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"
#include "debug.h"


Command make_command(const char* string){

    //we assume there is a ) at the end
    size_t input_len = strlen(string);

    Argument args[MAX_ARGC] = {0};
    int argc = 0;


    size_t last_arg_start = 0;
    size_t index=0;

    while (index <= input_len ){
        if (string[index] == ' ' || string[index] == '\0' || string[index] == ')'){
            //printf("world is between %d and %d\t", last_arg_start, index);
            size_t word_lenght = index-last_arg_start;
            char* word = malloc(word_lenght+1);
            word[word_lenght] = '\0';
            strncpy(word, string+last_arg_start, word_lenght);
            
            if (strcmp(word, ")") == 0){
                free(word);
                break;
            }

            //printf("word is %s\n", word);
            Argument arg = {.litteral=word, .cmd=NULL};
            args[argc] = arg;
            argc+=1;

            while (string[index] == ' ') index++;
            last_arg_start = index;
        }
        if (string[index] == '('){
            Command* command = malloc(sizeof(Command));
            *command = make_command(string+index+1);

            Argument arg = {.litteral=NULL, .cmd=command};
            while (string[index] != ')') index++;
            index++;
            last_arg_start = index;

            args[argc] = arg;
            argc+=1;
        }

        //if (string[inde])

        index++;
    }

    Argument* final = malloc(sizeof(Argument)*argc);
    for (int i=0; i<argc; i++){
        final[i] = args[i];
    }
    Command shit = {final, argc};
    return shit;
}

void free_argument(Argument* arg){
    if (arg->litteral){
        free(arg->litteral);
    }
    if (arg->cmd){
        free_command(arg->cmd);
        free(arg->cmd);
    }
}

void free_command(Command* cmd){
    //don't free the command itself, but its inside YES
    for (int i=0; i<cmd->argc; i++){
        free_argument(&cmd->args[i]);
    }
    free(cmd->args);
}
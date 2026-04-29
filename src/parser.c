
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "parser.h"


void debug_arg(Argument* arg){
    if (arg->litteral){
        printf("arg is string: %s\n", arg->litteral);
    }else{
        printf("arg is cmd: %p\n", (void*)arg->cmd);
    }
}

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

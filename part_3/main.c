#include <stdio.h>
#include "defs.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define HELP "help"
#define HIRE "hire"
#define FIRE "fire"
#define FIREALL "fireall"

#define LIST "list"
#define EXIT "exit"

void cse320_print(char* message);
void printHelpInformation();
void hire(int N);
void fireall();
void exitthis();
void list();
void fire(int id);
int isNumber(char* input);

char *my_argv[10];

void free_argv(){
    for(int i = 0; my_argv[i] != NULL; i++) {
        memset(my_argv[i], 0, strlen(my_argv[i]));
        my_argv[i] = NULL;
        free(my_argv[i]);
    }
}

void toString(char* from, char* to){
    while(*to != '\0'){
        *from = *to;
        to++;
        from++;
    }
    *from = 0;
}

void sigint_handler(int sig) 
{
    cse320_print("\n");
    fflush(stdout);
    exitthis();
    exit(0);
}


void fill_argv(char *tmp_argv){
    char *input = tmp_argv;
    int i = 0;
    char* tmp = malloc(sizeof(char) * strlen(input) + 1);
	tmp[0] = '\0';
    while(*input != '\0') { // while its not at the end yet
        if (*input == ' '){ //everytime it see a space
            if(my_argv[i] == NULL)
                my_argv[i] = (char *)malloc(sizeof(char) * strlen(tmp) + 1);
            else
                my_argv[i] = NULL;
            toString(my_argv[i], tmp);
            memset(tmp, 0, strlen(tmp) + 1);
            i++;
        }
        else{
            strncat(tmp, input, 1);
        }
        input++;
    }

    my_argv[i] = malloc(sizeof(char) * strlen(tmp) + 1);
    toString(my_argv[i], tmp);
    free(tmp);
}

int isEqual(char* string1, char* string2){ //while it's not empty, and they are the same at each
    while((*string1 != '\0') && (*string2 != '\0') && (*string1 == *string2))    {
        string1++;
        string2++;
    }
    return (*string1  == *string2);
}


int main(void) {
    while(1) {
        printf("shell> ");
        char* usercommand = malloc(255* sizeof(char));
        scanf("%[^\n]%*c", usercommand);
        size_t len = strlen(usercommand);
        char *userCommand = malloc(len + 1 + 1); /* one for extra char, one for trailing zero */
        toString(userCommand, usercommand);
        free(usercommand);
        userCommand[len] = '\0';
        fill_argv(userCommand);
        free(userCommand);

        if (strcmp(my_argv[0], HELP) == 0 && my_argv[1] == NULL){
            printHelpInformation();
            free_argv();
        }
        else if(strcmp(my_argv[0], HIRE) == 0 && my_argv[1] != NULL && isNumber(my_argv[1]) == 0 && my_argv[2] == NULL){
            hire(atoi(my_argv[1]));
            free_argv();
        }
        else if(strcmp(my_argv[0], FIRE) == 0 && my_argv[1] != NULL && isNumber(my_argv[1]) == 0 && my_argv[2] == NULL){
            fire(atoi(my_argv[1]));
            free_argv();
        }
        else if(strcmp(my_argv[0], LIST) == 0 && my_argv[1] == NULL){
            list();
            free_argv();
        }
        else if(strcmp(my_argv[0], FIREALL) == 0 && my_argv[1] == NULL){
            fireall();
            free_argv();
        }
        else if(strcmp(my_argv[0], EXIT) == 0 && my_argv[1] == NULL) {
            exitthis();
            free_argv();
            break;
        }
        else {
            cse320_printf("Invalid Command.\nList of available commands is available through the \"help\" command. \n");
            free_argv();
        }
    }
    free_argv();
    return 0;
}


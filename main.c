#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>
#include "pthread.h"

#define IN "in"
#define OUT "out"
#define INFO "info"
#define START "start"
#define EXIT "exit"

#define OPEN "OPEN"
#define CLOSE "CLOSED"

int museum_ds[5]; //data structure for 5 museums - their index + 1 is the museum #
char* museum_ds_status[5];

int started = 0;
int exiting = 0;

void fill_struct(){
    for(int i = 0; i < 5; i++){
        museum_ds[i] = 1;
        museum_ds_status[i] = OPEN;
    }
}

void free_struct(){
    for(int i = 0; museum_ds_status[i] != NULL; i++) {
        //memset(museum_ds_status[i], 0, strlen(museum_ds_status[i]));
        museum_ds_status[i] = NULL;
        free(museum_ds_status[i]);
    }
}

void visitor_in(int N){
    for(int i = 1; i <= N; i++){
        if(i == N && museum_ds_status[i-1] == OPEN)
            museum_ds[i-1] = museum_ds[i-1]+1;
    }
}

void* t_in(void* N){
    visitor_in((intptr_t)N);
    pthread_exit(0);
}

void visitor_out(int N){
    for(int i = 1; i <= N; i++){
        if(i == N) {
            if(museum_ds[N-1] == 0)
                break;
            else
                museum_ds[N - 1] = museum_ds[i - 1] - 1;
        }
    }
}

void* t_out(void* N){
    visitor_out((intptr_t)N);
    pthread_exit(0);
}

void museum_info(){
    for(int i = 0; i < 5; i++)
        printf("%d:%d:%s\n", i+1, museum_ds[i], museum_ds_status[i]);
}

void* t_info(){
    museum_info();
    pthread_exit(0);
}

void museum_close(int N){
    museum_ds_status[N] = CLOSE;
}

void museum_clean(){
    for(int i = 0; i < 5; i++){
        if(museum_ds[i] != 0)
            museum_ds[i] = 0;
        museum_close(i);
    }
    free_struct();
}

char *my_argv[10];


void free_argv(){
    for(int i = 0; my_argv[i] != NULL; i++) {
        memset(my_argv[i], 0, strlen(my_argv[i]));
        my_argv[i] = NULL;
        free(my_argv[i]);
    }
}

int done = 0;

void *start_thread(){
    while(done == 0) {
        int count = 0;
        if(exiting == 1) {
            pthread_cancel(pthread_self());
            return NULL;
        }
        for(int i = 0; i < 5; i++)
            if(museum_ds_status[i] == OPEN && museum_ds[i] == 0)
                museum_close(i);
        for(int i = 0; i < 5; i++){
            if(museum_ds_status[i] == CLOSE)
                count++;
        }
        if(count == 5)
            done = 1;
        sleep(3);
    }
    //if it reached here that means all of the museums are closed :)
    return 0;
}



int isNumber(char* input){
    if(input == NULL)
        return 1;
    int length = strlen (input);
    for (int i = 0; i < length; i++)
        if (!isdigit(input[i]))
            return 1;
    return 0;
}




void toString(char* from, char* to){
    while(*to != '\0'){
        *from = *to;
        to++;
        from++;
    }
    *from = 0;
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

int main(void) {
    fill_struct();
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
        if(strcmp(my_argv[0], IN) == 0 && my_argv[1] != NULL && isNumber(my_argv[1]) == 0 && isNumber(my_argv[2]) == 0 && atoi(my_argv[1]) > 0 && atoi(my_argv[1]) <= 5  && my_argv[3] == NULL){
            for(int i = 0; i < atoi(my_argv[2]); i++){
                pthread_t thread;
                pthread_create(&thread, NULL, t_in, (void*)(intptr_t)atoi(my_argv[1]));
            }

            free_argv();
        }
        else if(strcmp(my_argv[0], OUT) == 0 && my_argv[1] != NULL && isNumber(my_argv[1]) == 0 && isNumber(my_argv[2]) == 0 && atoi(my_argv[1]) > 0 && atoi(my_argv[1]) <= 5  && my_argv[3] == NULL && atoi(my_argv[2])<=museum_ds[atoi(my_argv[1])-1]){
            for(int i = 0; i < atoi(my_argv[2]); i++){
                pthread_t thread;
                pthread_create(&thread, NULL, t_out, (void*)(intptr_t)atoi(my_argv[1]));
                pthread_join(thread, NULL);
            }
            free_argv();
        }
        else if (strcmp(my_argv[0], INFO) == 0 && my_argv[1] == NULL){
            pthread_t thread;
            pthread_create(&thread, NULL, t_info, NULL);
            pthread_join(thread, NULL);
            free_argv();
        }
        else if(strcmp(my_argv[0], START) == 0 && my_argv[1] == NULL && started == 0){
            started = 1;
            pthread_t thread;
            pthread_create(&thread, NULL, &start_thread, NULL);
            //need to check if start been called already to not let it be called again while it is running;
            free_argv();
        }
        else if(strcmp(my_argv[0], EXIT) == 0 && my_argv[1] == NULL && exiting == 0) {
            exiting = 1;
            museum_clean();
            free_argv();
            pthread_exit(NULL);
        }
        else {
            free_argv();
        }
    }
}


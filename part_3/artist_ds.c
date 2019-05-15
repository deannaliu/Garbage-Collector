#include "defs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

// Assigned = 1 //
// Waiting = 0 //

typedef struct artists{
    pthread_t tid;
    int sig;
    struct artists* next;
}artist;

artist* head = NULL;

int isNumber(char* input){
    int length = strlen (input);
    for (int i = 0; i < length; i++)
        if (!isdigit(input[i]))
            return 1;
    return 0;
}

void printHelpInformation(){
    cse320_printf("%-30s \t %s\n", "help:", "You are here.");
    cse320_printf("%-30s \t %s\n", "hire [N]:", "Hire [N] number of artists to do nothing.");
    cse320_printf("%-30s \t %s\n", "fire [X]:", "Fire artist [X].");
    cse320_printf("%-30s \t %s\n", "fireall:", "Fire all the artists.");
    cse320_printf("%-30s \t %s\n", "list:", "Lists all current artists and their status.");
    cse320_printf("%-30s \t %s\n", "exit:", "Exits the program");
}

void toString(char* to, char* from);

void* threadFunction(){
    pthread_detach(pthread_self());
    return NULL;
}
void hire(int N){
    pthread_t thread_id;
    for(int i = 0; i < N; i++){
        pthread_create(&thread_id, NULL, threadFunction, NULL);
        //pthread_detach(thread_id);
        artist* current = head;
        artist* newArtist = malloc(sizeof(artist));
        newArtist->tid = thread_id; //stores the process value
        newArtist->sig = thread_id; //stores the int value
        if(current == NULL){
            newArtist->next = head;
            head = newArtist;
        }
        else {
            while(current->next != NULL) {
                current = current->next;
            }
            newArtist->next = current->next;
            current->next = newArtist;
        }
    }
}

void list(){
    artist* current = head;
    char *str = (char*)malloc(20 * sizeof(char));
    while(current!=NULL) {
        sprintf(str, "%u %s\n", current->sig, "WAITING");
        cse320_print(str);
        current = current->next;
    }
    free(current);
    free(str);
}

void freeList(){
    artist* tmp;
    while (head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
    head = NULL;
}

void fireall(){
    artist* current = head;
    while (current != NULL) {
        pthread_cancel(current->tid);
        current = current->next;
    }
    freeList();
}

void fire(int id){
    if(head == NULL)
        return;
    if(head->sig == id){
        artist* ptr = head;
        head = head->next;
        ptr->next = NULL;
        if(ptr == head)
            head = NULL;
        free(ptr);
        return;
    }

    artist* current = head->next;
    artist* prev = head;
    artist* tmp = NULL;

    while(current != NULL && prev != NULL){
        if(current->sig == id){
            pthread_cancel(current->tid);
            tmp = current;
            prev->next = current->next;
            free(tmp);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void exitthis(){
    fireall();
    pthread_exit(NULL);
}





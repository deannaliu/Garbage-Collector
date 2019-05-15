#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define FILE1 "file1.dat"
#define FILE2 "file2.dat"
#define FILE3 "file3.dat"

struct number_counter{
    // global values //
    int file1_counter;
    int file2_counter;
    int file3_counter;
    int no0;
    int no1;
    int no2;
};

struct number_counter* num;

int isNumber(char* input){
    int length = strlen (input);
    for (int i = 0; i < length; i++)
        if (!isdigit(input[i]))
            return 1;
    return 0;
}

int split(char* str){
    int i = 0;
    char* token = strtok(str, " ");
    while (token != NULL) {
        if(atoi(token) == 0)
            num->no0 = num->no0 + 1;
        else if(atoi(token) == 1)
            num->no1 = num->no1 + 1;
        else if(atoi(token) == 2)
            num->no2 = num->no2 + 1;
        token = strtok(NULL, " ");
        i++;
    }
    return i;
}

void count_file(int no){
    FILE *fp1;
    char *n = malloc(sizeof(int));
    if(no == 1)
        fp1 = fopen(FILE1, "r");
    else if(no == 2)
        fp1 = fopen(FILE2, "r");
    else if(no == 3)
        fp1 = fopen(FILE3, "r");
    else
        exit(0);
    if(fp1!=NULL)
        fscanf(fp1, "%[^\n]", n);
    else{
	printf("Invalid File.\n");
	exit(0);
    }
    if(isNumber(n) == 1 || atoi(n) < 0){
	printf("Invalid N.\n");
        exit(0);
    }
    if(no == 1)
        num->file1_counter = atoi(n);
    else if(no == 2)
        num->file2_counter = atoi(n);
    else if(no == 3)
        num->file3_counter = atoi(n);

    char* str = NULL;
    str = malloc(255*sizeof(char));
    while(fgets(str, 255, fp1) != NULL) {
    }
    int cnt = split(str);

    fclose(fp1);
    free(n);
    free(str);
    if(!((no == 1 && num->file1_counter == cnt) || (no == 2 && num->file2_counter == cnt) || (no == 3 && num->file3_counter == cnt))){
        printf("N does not match number of numbers. Please try again.\n"); //when number of numbers != big N
	exit(1);
}
}

void *file_result(void *param){
    if((intptr_t)param == 1)
        count_file(1);
    else if((intptr_t)param == 2)
        count_file(2);
    else if((intptr_t)param == 3)
        count_file(3);
    else
        exit(0);
    pthread_exit(0);
}

void fill_struct(){
    num = malloc(sizeof(struct number_counter));
    num->file1_counter=0;
    num->file3_counter=0;
    num->file2_counter=0;
    num->no1=0;
    num->no2=0;
    num->no0=0;
}

void print_struct(){
    printf("%d %d %d\n", num->no0, num->no1, num->no2);
}

int main(int argc, char *argv[]) {
    fill_struct();
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for(int i = 1; i <= 3; i++) {
        pthread_t thread;
        pthread_create(&thread, &attr, file_result, (void *) (intptr_t) (i));
        pthread_join(thread, NULL);
    }
    print_struct();
    free(num);
    return 0;
}




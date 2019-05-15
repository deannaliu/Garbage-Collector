#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

int num;

int fibonacci (int x){
    if (x <= 2)
        return 1;
    return fibonacci(x-1) + fibonacci(x-2);
}

void *t(void *param){
    num = (intptr_t)fibonacci((intptr_t)param);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int count;
    pthread_attr_t attr;

    if (argc != 2)
        return -1;

    count = atoi(argv[1]); 

    if (count < 1 || count > INT_MAX)
        return -1;

    pthread_attr_init(&attr);

    for(int i = 1; i <= count; i++){
        pthread_t thread;
        pthread_create(&thread,&attr,t,(void*)(intptr_t)i);
        pthread_join(thread,NULL);
        if(i == count)
            printf("%d\n", num);
    }
    return 0;
}




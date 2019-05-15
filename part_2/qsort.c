#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct arr {
    int* list;
    int start;
    int end;
} p;

int cnt = 0;
int arr[1000];

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(int list[], int start, int end) {
    int i;
    int s = start + 1;
    for(i = start + 1; i <= end; i++) {
        if(list[i] < list[start]) {
            swap(&list[s], &list[i]);
            s++;
        }
    }
    return s - 1;
}

void sort_helper(int list[], int start, int end) {
    if(end <= start)
        return;
    int p = partition(list, start, end);
    swap(&list[p], &list[start]);
    sort_helper(list, start, p - 1);
    sort_helper(list, p + 1, end);
}

void* p_quicksort(void* pointer) {
    p params = *((p*) pointer);
    int p_index, end = params.end, start = params.start;
    int* list = params.list;
    if(end <= start)
        return NULL;
    p_index = partition(list, start, end);
    swap(&list[p_index], &list[start]);
    sort_helper(list, start, p_index - 1);
    sort_helper(list, p_index + 1, end);
    return NULL;
}

void fill_params(p* p_struct, int* list, int start, int end) {
    p_struct->list = list;
    p_struct->start = start;
    p_struct->end = end;
}

void parallel_quicksort(int list[], int size) {
    p s, g;
    pthread_t s_thread, g_thread;
    int i;
    if(size == 0)
        return;
    i = partition(list, 0, size - 1);
    swap(&list[i], &list[0]);

    fill_params(&s, list, 0, i - 1);
    fill_params(&g, list, i + 1, size - 1);

    pthread_create(&s_thread, NULL, p_quicksort, &s);
    pthread_create(&g_thread, NULL, p_quicksort, &g);

    pthread_join(s_thread, NULL);
    pthread_join(g_thread, NULL);
}

void print(int array[], int size) {
    int i;
    for(i = 0; i < size - 1; i++)
        printf("%d ", array[i]);
    printf("%d\n", array[i]);
}

int isNumber(char* input){
    size_t length = strlen (input);
    for (int i = 0; i < length; i++)
        if (!isdigit(input[i]))
            return 1;
    return 0;
}

int split(char* str){
    int i = 0;
    int j = 0;

    char* token = strtok(str, " ");

    while (token != NULL) {
        arr[j] = atoi(token);
        token = strtok(NULL, " ");
        j++;
        i++;
    }
    return i;
}

int main(int argc, const char * argv[]) {
    FILE *fp;
    fp = fopen(argv[1], "r");
    char *n = malloc(sizeof(int));
    if(fp!=NULL)
        fscanf(fp, "%[^\n]", n);
    else{
	printf("Invalid File\n");
        exit(0);
    }
    char* str = NULL;
    str = malloc(255*sizeof(char));
    while(fgets(str, 255, fp) != NULL) {}
    cnt = split(str);
    if(atoi(n)!=cnt)
        exit(0);
    fclose(fp);
    parallel_quicksort(arr, cnt);
    print(arr, cnt);
	free(n);
	free(str);
    return 0;
}

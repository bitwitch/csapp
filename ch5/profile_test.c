#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// build an random array of length len
int *make_array(int len) {
    int *a = malloc(len * sizeof(int));

    time_t t;
    srand((unsigned) time(&t));

    for (int i=0; i<len; i++)
        a[i] = rand();

    return a;
}

void sort_array(int *arr, int len) {
    int swapped = 1;
    int i, tmp;
    while (swapped) {
        swapped = 0;
        for (i=0; i<len-1; i++) {
            if (arr[i] > arr[i+1]) {
                tmp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = tmp;
                swapped = 1;
            }
        }
    }
}

void clear_array(int *arr) {
    free(arr);
}


int main() {
    int i;
    int *a = NULL;
    int len;

    for (i=0; i<1569; i++) {
        len = i+1;
        a = make_array(len);
        sort_array(a, len);
        clear_array(a);
    }
    printf("\nDone.\n");

    return 0;
}

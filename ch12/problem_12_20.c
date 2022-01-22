#include <stdio.h>
#include "csapp.h"

#define MAX_READERS 8
#define QUIT_THRESHOLD 65536

static int reads, writes;
sem_t mutex;
sem_t slots;

void *reader(void *vargp) {
    while (1) {
        P(&slots);
        P(&mutex);

        /* critical section */
        /* reading happens */
        reads++;
        if (reads >= QUIT_THRESHOLD) {
            printf("reads: %d, writes: %d\n", reads, writes);
            exit(0);
        }

        V(&mutex);
        V(&slots);
    }
}

void *writer(void *vargp) {
    while (1) {
        P(&mutex);
        P(&slots);

        /* critical section */
        /* writing happens */
        writes++;
        if (writes >= QUIT_THRESHOLD) {
            printf("reads: %d, writes: %d\n", reads, writes);
            exit(0);
        }

        V(&mutex);
        V(&slots);
    }

}

int main(void) {

    Sem_init(&mutex, 0, 1);
    Sem_init(&slots, 0, 2*MAX_READERS);
    pthread_t tids[2*MAX_READERS];

    for (int i=0; i<2*MAX_READERS; i++) {
        if (i % 2 == 0)
            Pthread_create(tids+i, NULL, reader, NULL);
        else 
            Pthread_create(tids+i, NULL, writer, NULL);
    }


    Pthread_exit(NULL);

    return 0;
}

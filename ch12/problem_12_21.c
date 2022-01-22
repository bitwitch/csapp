#include <stdio.h>
#include "csapp.h"

#define NTHREADS 16
#define QUIT_THRESHOLD 65536

static int readers, writers_waiting, reads, writes;
sem_t w, r, write_wait, read_wait;

void *reader(void *vargp) {
    while (1) {
        P(&write_wait);
        int w_waiting = writers_waiting;
        V(&write_wait);

        if (w_waiting > 0) {
            P(&read_wait);
            V(&read_wait);
        }

        P(&r);
        readers++;
        reads++;
        if (readers == 1)
            P(&w); /* if a writer is writing, wait */

        if (reads >= QUIT_THRESHOLD) {
            printf("reads: %d, writes: %d\n", reads, writes);
            exit(0);
        }
        V(&r);

        P(&r);
        readers--;
        if (readers == 0)
            V(&w);
        V(&r);
    }
}

void *writer(void *vargp) {
    while (1) {
        usleep(10);
        P(&write_wait);
        writers_waiting++;
        if (writers_waiting == 1)
            P(&read_wait);
        V(&write_wait);

        P(&w);

        writes++;
        if (writes >= QUIT_THRESHOLD) {
            printf("reads: %d, writes: %d\n", reads, writes);
            exit(0);
        }

        V(&w);

        P(&write_wait);
        writers_waiting--;
        if (writers_waiting == 0)
            V(&read_wait);
        V(&write_wait);
    }
}

int main(void) {
    Sem_init(&w, 0, 1);
    Sem_init(&r, 0, 1);
    Sem_init(&write_wait, 0, 1);
    Sem_init(&read_wait, 0, 1);

    pthread_t tids[NTHREADS];

    for (int i=0; i<NTHREADS; i++) {
        if (i % 2 == 0)
            Pthread_create(tids+i, NULL, writer, NULL);
        else 
            Pthread_create(tids+i, NULL, reader, NULL);
    }

    Pthread_exit(NULL);
    return 0;
}

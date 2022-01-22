#include "csapp.h"
void *thread(void *vargp);

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage %s <nthreads>\n", argv[0]);
        exit(1);
    }

    int nthreads = atoi(argv[1]);
    pthread_t tids[nthreads];

    for (int i=0; i<nthreads; i++) 
        Pthread_create(tids+i, NULL, thread, NULL);

    for (int i=0; i<nthreads; i++) 
        Pthread_join(tids[i], NULL);

    exit(0);
}

void *thread(void *vargp) {
    printf("Hello, world!\n");
    return NULL;
}                                           


#include "csapp.h"

typedef struct {
    char *s;
    int size;
    FILE *stream;
    pthread_t sleeper;
} fgets_thread_args_t;

void *fgets_thread(void *vargp) {
    fgets_thread_args_t *args = vargp;
    Fgets(args->s, args->size, args->stream);
    Pthread_cancel(args->sleeper);
}

void *sleep_thread(void *vargp) {
    sleep(5);
}

char *tfgets(char *s, int size, FILE *stream) {
    pthread_t sleeper, fgetter;

    Pthread_create(&sleeper, NULL, sleep_thread, NULL);

    fgets_thread_args_t args = {s, size, stream, sleeper};
    Pthread_create(&fgetter, NULL, fgets_thread, (void *)&args);

    void *sleeper_status = NULL;
    Pthread_join(sleeper, &sleeper_status);

    if (sleeper_status == PTHREAD_CANCELED)
        Pthread_join(fgetter, NULL);
    else {
        Pthread_cancel(fgetter);
        return NULL;
    }

    return s;
}

int main() {
    char buf[MAXLINE];

    if (tfgets(buf, MAXLINE, stdin) == NULL)
        printf("BOOM!\n");
    else
        printf("%s\n", buf);

    exit(0);
}



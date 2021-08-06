#include <stdio.h>
#include <unistd.h>
#include "csapp.h"


char *tfgets(char *restrict s, int n, FILE *restrict stream) {
    pid_t cpid;
    int status;

    // spawn a child to read from stream, with a 5 second alarm
    if ((cpid = Fork()) == 0) {
        alarm(5);
        fgets(s, n, stream);
        printf("from child %p, %s\n", &s, s);
        alarm(0);
        exit(0);
    }

    if (waitpid(-1, &status, 0) > 0) {
        if (WIFEXITED(status)) {
            printf("normal\n");
            printf("from parent %p, %s\n", &s, s);
            return s;
        } 

        if (WIFSIGNALED(status)) {
            printf("signaled\n");
            return NULL;
        }
    }

    perror("waitpid");
    return NULL;
}


int main() {

    char buf[256];
    /*fgets(buf, 256, stdin);*/
    /*printf("got: %s\n", buf);*/

    tfgets(buf, 256, stdin);
    printf("got: %s\n", buf);

    return 0;
}

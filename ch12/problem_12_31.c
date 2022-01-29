#include "csapp.h"

sigjmp_buf env;

void sigchld_handler(int signum) {
    siglongjmp(env, -1);
}

char *tfgets(char *s, int size, FILE *stream) {
    if (sigsetjmp(env, 1) != 0) {
        return NULL;
    }

    Signal(SIGCHLD, sigchld_handler);

    pid_t cpid;
    if ((cpid = Fork()) == 0) {
        sleep(5);
        exit(0);
    }

    return Fgets(s, size, stream);
}

int main() {
    char buf[MAXLINE];

    if (tfgets(buf, MAXLINE, stdin) == NULL)
        printf("BOOM!\n");
    else
        printf("%s\n", buf);

    exit(0);
}


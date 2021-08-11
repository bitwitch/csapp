#include <stdio.h>
#include "csapp.h"

static int alarm_triggered = 0;
jmp_buf env;

void sigalarm_handler(int sig) {
    alarm_triggered = 1;
    longjmp(env, 0);
}

char *tfgets(char *restrict s, int n, FILE *restrict stream) {
    pid_t pid;
    int status;

    setjmp(env);

    if (alarm_triggered) return NULL;

    Signal(SIGALRM, sigalarm_handler);

    alarm(5);
    fgets(s, n, stream);
    alarm(0);

    return s;
}


int main() {
    char buf[256];

    tfgets(buf, 256, stdin);
    printf("Entered: %s\n", buf);

    return 0;
}



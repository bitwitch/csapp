#include "csapp.h"

char *tfgets(char *s, int size, FILE *stream) {
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(STDIN_FILENO, &read_set);

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    Select(STDIN_FILENO+1, &read_set, NULL, NULL, &timeout);

    if (FD_ISSET(STDIN_FILENO, &read_set))
        return Fgets(s, size, stream);

    return NULL;
}

int main() {
    char buf[MAXLINE];

    if (tfgets(buf, MAXLINE, stdin) == NULL)
        printf("BOOM!\n");
    else
        printf("%s\n", buf);

    exit(0);
}



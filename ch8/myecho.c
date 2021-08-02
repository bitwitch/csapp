#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    int i;
    char *s;

    printf("Command-line arguments:\n");
    for (i=0; i<argc; i++) {
        printf("    argv[%2d]: %s\n", i, argv[i]);
    }

    i = 0;
    printf("Environment variables:\n");
    while ( (s = envp[i++]) ) {
        printf("    envp[%2d]: %s\n", i, s);
    }

    return 0;
}

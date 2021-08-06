/* Problem 8.24 */

#include "csapp.h"
#define N 5

int main() {
    int status, i;
    pid_t pid;

    for (i=0; i<N; i++)
        if ((pid = Fork()) == 0) {
            int *ro_data = (int *) 0x1;
            *ro_data = 69;
            return 0;
        }

    while ((pid = waitpid(-1, &status, 0)) > 0) {
        if (WIFEXITED(status))
            printf("child %d terminated normally with exit status=%d\n", pid, WEXITSTATUS(status));
        else if (WIFSIGNALED(status)) {
            char *error_string[64];
            sprintf(error_string, "child %d terminated by signal %d", pid, status);
            psignal(status, error_string);
        }
    }

    if (errno != ECHILD)
        unix_error("waitpid error");

    exit(0);
}


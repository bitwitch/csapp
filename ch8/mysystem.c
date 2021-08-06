/* Problem 8.22 */

#include <csapp.h>

int mysystem(const char *command) {
    if (command == NULL) {
        return 1;
    }

    pid_t cpid, w;
    int status;

    if ((cpid = Fork()) == 0) {
        return execl("/bin/sh", "sh", "-c", command, (char *) NULL);
    }

    printf("child pid: %d\n", cpid);

    w = waitpid(cpid, &status, 0);
    if (w == -1) {
        errno = ECHILD;
        return -1;
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    if (WIFSIGNALED(status))
        return WTERMSIG(status);

    return 1;
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Enter a command as an argument.\n");
        return 0;
    }
    int exit_code = mysystem(argv[1]);
    printf("Exit code: %d\n", exit_code);
    return 0;
}

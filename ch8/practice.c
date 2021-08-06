#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

/* Practice 8.5 */
unsigned snooze(unsigned secs) {
    unsigned time_left;
    time_left = sleep(secs);
    printf("Slept for %u of %u secs.\n", secs - time_left, secs);
    return time_left;
}

/* Practice 8.7 */
void sigint_handler(int sig) {
    int saved_errno = errno;
    write(1, "\n", 1);
    errno = saved_errno;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Expected 1 command line argument, got none.\n");
        return 1;
    }

    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        printf("Failed to install signal handler sigint_handler.\n");
        exit(1);
    }

    printf("Started...\n");
    int secs = atoi(argv[1]);
    unsigned time_left = snooze(secs);
    if (time_left == 0) {
        printf("Slept full time.\n");
    }

    return 0;
}

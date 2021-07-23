#include <unistd.h>
#include <stdio.h>

/* Practice 8.4 */
unsigned snooze(unsigned secs) {
    unsigned time_left;
    time_left = sleep(secs);
    printf("Slept for %u of %u secs.\n", secs - time_left, secs);
    return time_left;
}

int main() {
    printf("Started...\n");
    snooze(13);
    printf("Done.\n");
    return 0;
}

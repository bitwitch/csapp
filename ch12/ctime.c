
#include <stdio.h>
#include <time.h>
#include <string.h>


int main(void) {

    time_t now = time(0);
    char *time_string = ctime(&now);

    now = time(0);
    char *diff_time_string = ctime(&now);

    printf("now is %s\n", time_string);
    printf("addr of time_string:     %p\n", time_string);
    printf("addr of new_time_string: %p\n", diff_time_string);


    strncpy(diff_time_string, "farts", 5);

    printf("now is %s\n", time_string);

    return 0;
}

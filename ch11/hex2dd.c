#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Practice Problem 11.2 */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <hex>\n", argv[0]);
        exit(0);
    }

    unsigned int hex = htonl(strtol(argv[1], NULL, 0));

    char dd[INET_ADDRSTRLEN];
    if (!inet_ntop(AF_INET, &hex, dd, INET_ADDRSTRLEN)) {
        fprintf(stderr, "Failed to convert hex to dd: %s\n", strerror(errno));
        exit(0);
    }

    printf("%s\n", dd);

    return 0;
}

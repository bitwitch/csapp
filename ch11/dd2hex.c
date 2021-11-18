#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Practice Problem 11.3 */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <dotted_decimal>\n", argv[0]);
        exit(0);
    }

    unsigned int hex;
    int status = inet_pton(AF_INET, argv[1], &hex);
    if (status == 0) {
        fprintf(stderr, "Argument provided is not a valid ip address\n");
    } else if (status == -1) {
        fprintf(stderr, "Failed to convert dd to hex: %s\n", strerror(errno));
        exit(0);
    }

    hex = ntohl(hex);

    printf("%#x\n", hex);

    return 0;
}

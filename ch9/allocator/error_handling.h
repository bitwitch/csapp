#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

#endif

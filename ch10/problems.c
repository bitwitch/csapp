#include "../csapp.h"

/* Problem 10.7 */
int cpfile() {
    int n;
    char buf[MAXBUF];

    while ((n = Rio_readn(STDIN_FILENO, buf, MAXBUF)) != 0)
        Rio_writen(STDOUT_FILENO, buf, n);

    return 0;
}

/* Problem 10.8 */
int fstatcheck(int fd) {
    struct stat stat;
    char *type, *readok;

    Fstat(fd, &stat);
    if (S_ISREG(stat.st_mode))     /* Determine file type */
        type = "regular";
    else if (S_ISDIR(stat.st_mode))
        type = "directory";
    else
        type = "other";
    if ((stat.st_mode & S_IRUSR)) /* Check read access */
        readok = "yes";
    else
        readok = "no";

    printf("type: %s, read: %s\n", type, readok);
    return 0;
}

/* Problem 10.10 */
int cpfile2(char *infile) {
    int n;
    rio_t rio;
    char buf[MAXLINE];
    int fd = -1;

    if (strlen(infile) > 0) {
        fd = Open(infile, O_RDONLY, 0);
        Dup2(fd, STDIN_FILENO);
    }

    Rio_readinitb(&rio, STDIN_FILENO);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0)
        Rio_writen(STDOUT_FILENO, buf, n);

    if (fd != -1)
        Close(fd);

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <command> [<arg>]\n", argv[0]);
        exit(0);
    }

    if (!strncmp(argv[1], "cpfile", 256))
        return cpfile();

    if (!strncmp(argv[1], "cpfile2", 256)) {
        char *infile = "";
        if (argc >= 3)
            infile = argv[2];
        return cpfile2(infile);
    }

    if (!strncmp(argv[1], "fstatcheck", 256)) {
        if (argc < 3) {
            fprintf(stderr, "usage: %s <command> <file_descriptor>\n", argv[0]);
            exit(0);
        }
        int fd = atoi(argv[2]);
        return fstatcheck(fd);
    }

    return 0;
}

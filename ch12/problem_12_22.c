#include "csapp.h"

#define MAX_CONNECTIONS 64

size_t echo(int connfd);
void command(void);
int available_connection(int *connections, int length);

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    fd_set read_set, ready_set;
    int connections = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);

    FD_ZERO(&read_set);              /* Clear read set */ 
    FD_SET(STDIN_FILENO, &read_set); /* Add stdin to read set */ 
    FD_SET(listenfd, &read_set);     /* Add listenfd to read set */ 

    int maxfd = listenfd;

    while (1) {
        ready_set = read_set;
        Select(maxfd+1, &ready_set, NULL, NULL, NULL); 

        if (FD_ISSET(STDIN_FILENO, &ready_set)) 
            command(); /* Read command line from stdin */

        if (FD_ISSET(listenfd, &ready_set)) { 
            if (connections >= MAX_CONNECTIONS)
                continue;

            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            printf("[%d] accepted connection\n", connfd);

            FD_SET(connfd, &read_set);
            connections++;

            if (connfd > maxfd)
                maxfd = connfd;
        }

        for (int connfd = listenfd+1; connfd <= maxfd; connfd++) {
            if (FD_ISSET(connfd, &ready_set)) {
                if (echo(connfd) == 0) {
                    printf("[%d] closing connection\n", connfd);
                    Close(connfd);
                    FD_CLR(connfd, &read_set);
                    connections--;
                }
            }
        }
    }
}

size_t echo(int connfd) 
{
    size_t n = 0;
    char c, buf[MAXLINE];
    char *bufp = buf; 
    while ((n += Rio_readn(connfd, &c, 1)) > 0) {
        *bufp++ = c;
        if (c == '\n')
            break;
    }
    *bufp++ = '\0';

    if (n > 0)
        Rio_writen(connfd, buf, n);

    return n;
}


void command(void) {
    char buf[MAXLINE];
    if (!Fgets(buf, MAXLINE, stdin))
        exit(0); /* EOF */
    printf("%s", buf); /* Process the input command */
}



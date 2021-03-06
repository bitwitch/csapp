/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 */
#include "csapp.h"
#include <stdbool.h>

#define MAXHEADERS 64
#define MAX_REQUEST_BODY_BYTES 2048
#define MAX_HEADERS_BYTES 2048

typedef struct {
    char *name;
    char *value;
} http_header_t;

typedef struct {
    bool request_line_done, headers_done, body_done;  /* whether each part of request has been read yet */
    int is_static;
    int header_count;
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], query_string[MAXLINE];
    http_header_t headers[MAXHEADERS];
    char headers_buffer[MAX_HEADERS_BYTES];
    char body[MAX_REQUEST_BODY_BYTES];
} request_t;

typedef struct {                           /* Represents a pool of connected descriptors */ 
    int maxfd;                             /* Largest descriptor in read_set */   
    fd_set read_set;                       /* Set of all active read descriptors */
    fd_set ready_set;                      /* Subset of descriptors ready for reading  */
    int nready;                            /* Number of read ready descriptors from select */   
    int maxi;                              /* Highwater index into client array */
    int client_fds[FD_SETSIZE];            /* Set of active descriptors */
    rio_t client_rios[FD_SETSIZE];         /* Set of active read buffers */
    request_t client_requests[FD_SETSIZE]; /* Set of active requests */
} pool_t;

void init_pool(int listenfd, pool_t *p);
void add_client(int connfd, pool_t *p);
void check_clients(pool_t *p);
int byte_count = 0; /* Counts total bytes received by server */

void handle_request(int fd, rio_t *rio, request_t *request);
int parse_uri(char *uri, char *filename, char *query_string);
void serve_static(int fd, char *method, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *method, char *filename, char *query_string);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);
void reap_children(int signum);
char *find_header(request_t *request, const char* name);
void server_write(int fd, void *usrbuf, size_t n);
void read_requesthdrs(rio_t *rp, request_t *request);


int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    static pool_t pool;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    Signal(SIGCHLD, reap_children);
    Signal(SIGPIPE, SIG_IGN);

    listenfd = Open_listenfd(argv[1]);
    init_pool(listenfd, &pool);

    while (1) {
        /* get the ready fds */
        pool.ready_set  = pool.read_set;
        pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

        /* if it is a new connection, accept it */
        if (FD_ISSET(listenfd, &pool.ready_set)) {
            clientlen = sizeof(clientaddr);
            connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
            Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                        port, MAXLINE, 0);
            printf("Accepted connection from (%s, %s)\n", hostname, port);
            add_client(connfd, &pool);
        }

        /* service connected clients */
        check_clients(&pool);
    }

    return 0;
}

void init_pool(int listenfd, pool_t *p) 
{
    /* Initially, there are no connected descriptors */
    int i;
    p->maxi = -1;
    for (i=0; i<FD_SETSIZE; i++)
        p->client_fds[i] = -1;

    /* Initially, listenfd is only member of select read set */
    p->maxfd = listenfd;
    FD_ZERO(&p->read_set);
    FD_SET(listenfd, &p->read_set);
}



void add_client(int connfd, pool_t *p) 
{
    int i;
    p->nready--;
    for (i = 0; i < FD_SETSIZE; i++)  /* Find an available slot */
        if (p->client_fds[i] < 0) { 
            /* Add connected descriptor to the pool */
            p->client_fds[i] = connfd;                 
            Rio_readinitb(&p->client_rios[i], connfd); 

            /* Add the descriptor to descriptor set */
            FD_SET(connfd, &p->read_set); 

            /* initialize the request */
            memset(&p->client_requests[i], 0, sizeof(request_t));

            /* Update max descriptor and pool highwater mark */
            if (connfd > p->maxfd) 
                p->maxfd = connfd; 
            if (i > p->maxi)       
                p->maxi = i;       
            break;
        }
    if (i == FD_SETSIZE) /* Couldn't find an empty slot */
        app_error("add_client error: Too many clients");
}

void check_clients(pool_t *p) {
    int i, connfd, n;
    char line[MAXLINE]; 
    rio_t *rio;
    request_t *request;

    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
        connfd = p->client_fds[i];
        rio = &p->client_rios[i];
        request = &p->client_requests[i];

        /* If the descriptor is ready, echo a text line from it */
        if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) { 
            p->nready--;
            handle_request(connfd, rio, request);
            Close(connfd); 
            FD_CLR(connfd, &p->read_set); 
            p->client_fds[i] = -1;
        }
    }
}


/*
 * handle_request - handle one HTTP request/response transaction
 */
void handle_request(int fd, rio_t *rio, request_t *request) 
{
    struct stat sbuf;
    char line[MAXLINE];
    int n = Rio_readlineb(rio, line, MAXLINE);
    if (n  == 0)
        return;

    byte_count += n; 
    printf("Server received %d bytes on fd %d (%d total)\n",
            n, fd, byte_count);

    sscanf(line, "%s %s %s", request->method, request->uri, request->version);

    if (0 != strcasecmp(request->method, "GET") && 
        0 != strcasecmp(request->method, "HEAD") &&  
        0 != strcasecmp(request->method, "POST")) 
    {
        clienterror(fd, request->method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }

    read_requesthdrs(rio, request);

    /* Parse URI from request */
    request->is_static = parse_uri(request->uri, request->filename, request->query_string);
    if (stat(request->filename, &sbuf) < 0) {                     
        clienterror(fd, request->filename, "404", "Not found",
		    "Tiny couldn't find this file");
        return;
    }                                                    

    if (request->is_static) { /* Serve static content */
        if (0 != strcasecmp(request->method, "GET")) {
            clienterror(fd, request->method, "405", "Method Not Allowed",
                    "This method is not supported by the target resource");
            return;
        }
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { 
            clienterror(fd, request->filename, "403", "Forbidden",
                "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, request->method, request->filename, sbuf.st_size);
    }
    else { /* Serve dynamic content */
        if (0 != strcasecmp(request->method, "POST")) {
            clienterror(fd, request->method, "405", "Method Not Allowed",
                    "This method is not supported by the target resource");
            return;
        }

        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { 
            clienterror(fd, request->filename, "403", "Forbidden",
                "Tiny couldn't run the CGI program");
            return;
        }

        /* only accept application/x-www-form-urlencoded */
        char *content_type = find_header(request, "Content-Type");
        if (0 != strcasecmp(content_type, "application/x-www-form-urlencoded")) {
            clienterror(fd, content_type, "415", "Unsupported Content Type",
                "This server only accepts POST requests with Content-Type: application/x-www-form-urlencoded");
            return;
        }

        /* require Content-Length */
        char *content_length = find_header(request, "Content-Length");
        if (content_length == NULL) {
            clienterror(fd, content_length, "411", "Length Required",
                "This server requires POST requests to contain a Content-Length header");
            return;
        }
               
        /* Read request body */
        Rio_readlineb(rio, line, atoi(content_length)+1);
        strcpy(request->body, line);

        serve_dynamic(fd, request->method, request->filename, request->body);
    }
}

char *find_header(request_t *request, const char* name) {
    for (int i=0, count=request->header_count; i<count; i++) {
        if (0 == strcasecmp(name, request->headers[i].name))
            return request->headers[i].value;
    }
    return NULL;
}


/*
 * read_requesthdrs - read HTTP request headers
 */
void read_requesthdrs(rio_t *rp, request_t *request)
{
    char line[MAXLINE];
    ssize_t header_bytes;
    http_header_t *headers = request->headers;
    char *p = request->headers_buffer;

    header_bytes = Rio_readlineb(rp, line, MAXLINE);
    while(strcmp(line, "\r\n")) {
        strcpy(p, line);

        /* get header value */
        char *sep = strchr(p, ':');
        *sep = '\0';
        sep++;
        if (*sep == ' ') sep++;
        char *newline = strchr(sep, '\r');
        *newline = '\0';

        headers[request->header_count].name = p;
        headers[request->header_count].value = sep;
        
        p = newline + 1;
        request->header_count++;
        header_bytes = Rio_readlineb(rp, line, MAXLINE);
    }
    return;
}

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
int parse_uri(char *uri, char *filename, char *query_string) 
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {  /* Static content */ 
        strcpy(query_string, "");                             
        strcpy(filename, ".");                           
        strcat(filename, uri);                           
        if (uri[strlen(uri)-1] == '/')                   
            strcat(filename, "home.html");               
            return 1;
    }
    else {  /* Dynamic content */                        
        ptr = index(uri, '?');                           
        if (ptr) {
            strcpy(query_string, ptr+1);
            *ptr = '\0';
        }
        else 
            strcpy(query_string, "");                         
        strcpy(filename, ".");                           
        strcat(filename, uri);                           
        return 0;
    }
}

/*
 * serve_static - copy a file back to the client 
 */
void serve_static(int fd, char *method, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);    
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n", filesize);
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype);
    server_write(fd, buf, strlen(buf));

    if (!strcasecmp(method, "HEAD"))
        return;

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0); 
    srcp = (char *)Malloc(filesize);
    Rio_readn(srcfd, srcp, filesize);
    Close(srcfd);
    server_write(fd, srcp, filesize);
    Free(srcp);
}

 
void server_write(int fd, void *usrbuf, size_t n) {
    if (rio_writen(fd, usrbuf, n) != n) {
        if (errno == EPIPE) {
            /* ignore EPIPE */
        } else {
            unix_error("server_write error");
        }
    }
}


/*
 * get_filetype - derive file type from file name
 */
void get_filetype(char *filename, char *filetype) 
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else if (strstr(filename, ".mpg"))
        strcpy(filetype, "video/mpeg");
    else
        strcpy(filetype, "text/plain");
}  
/* $end serve_static */


/* SIGCHLD handler */
void reap_children(int signum) {
    pid_t cpid;
    do {
        cpid = wait(NULL);
    } while(cpid != -1 && errno != ECHILD);
}

/*
 * serve_dynamic - run a CGI program on behalf of the client
 */
void serve_dynamic(int fd, char *method, char *filename, char *query_string) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    server_write(fd, buf, strlen(buf));
  
    /* Real server would set all CGI vars here */
    setenv("METHOD", method, 1);
    setenv("QUERY_STRING", query_string, 1); 
    Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ 
    Execve(filename, emptylist, environ); /* Run CGI program */ 
}

/*
 * clienterror - returns an error message to the client
 */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    server_write(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    server_write(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    server_write(fd, buf, strlen(buf));
}

/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 */
#include "csapp.h"
#include <stdbool.h>

#define MAXHEADERS 64
#define MAX_REQUEST_BODY_BYTES 2048
#define MAX_HEADERS_BYTES 2048

#define CLIENT_ERROR        (-1)
#define EMPTY_REQUEST       (-2)
#define END_REQUEST_HEADERS (-3)
#define END_REQUEST_BODY    (-4)
#define SUCCESS             (0)


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
    fd_set read_ready_set;                 /* Subset of descriptors ready for reading  */
    fd_set write_set;                      /* Set of all active write descriptors */
    fd_set write_ready_set;                /* Subset of descriptors ready for writing  */
    int nready_read;                       /* Number of read ready descriptors from select */   
    int nready_write;                      /* Number of write ready descriptors from select */   
    int maxi;                              /* Highwater index into client array */
    int client_fds[FD_SETSIZE];            /* Set of active descriptors */
    rio_t client_rios[FD_SETSIZE];         /* Set of active read buffers */
    request_t client_requests[FD_SETSIZE]; /* Set of active requests */
} pool_t; 

void init_pool(int listenfd, pool_t *p);
void add_client(int connfd, pool_t *p);
void check_clients(pool_t *p);
int byte_cnt = 0; /* Counts total bytes received by server */

/*int prepare_response(connection_t *conn);*/
int parse_request_line(int connfd, request_t *request, char *line);
int parse_request_header(request_t *request, char *line);
int parse_request_body(int connfd, request_t *request, char *line);
int parse_uri(char *uri, char *filename, char *query_string);
void serve_static(int fd, char *method, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *method, char *filename, char *query_string);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);
void reap_children(int signum);
char *find_header(request_t *request, const char* name);
void server_write(int fd, void *usrbuf, size_t n);
void remove_connection(int *connections, int *con_count, int connfd);
int handle_request_errors(int connfd, request_t *request);


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
        pool.read_ready_set  = pool.read_set;
        pool.write_ready_set = pool.write_set;
        pool.nready_read = Select(pool.maxfd+1, &pool.read_ready_set, &pool.write_ready_set, NULL, NULL);

        /* if it is a new connection, accept it */
        if (FD_ISSET(listenfd, &pool.read_ready_set)) {
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
    p->nready_read--;
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

    for (i = 0; i <= p->maxi && (p->nready_read > 0 || p->nready_write > 0); i++) {
        connfd  =  p->client_fds[i];
        rio     = &p->client_rios[i];
        request = &p->client_requests[i];

        /* If the descriptor is read-ready, read a line of the request */
        if ((connfd >= 0) && (FD_ISSET(connfd, &p->read_ready_set))) {
            p->nready_read--;
            int rc;
            while (!request->request_line_done ||
                   (request->is_static && !request->headers_done) || 
                   (!request->is_static && !request->body_done))
            {
                n = Rio_readlineb(rio, line, MAXLINE);
                if (n == 0) break;
                byte_cnt += n; 
                printf("Server received %d bytes on fd %d (%d total)\n",
                        n, connfd, byte_cnt);
                rc = parse_request_line(connfd, request, line);
            }

            if (rc == SUCCESS) {
                /* move descriptor to write ready set */
                FD_CLR(connfd, &p->read_set);
                FD_SET(connfd, &p->write_set);

            } else  {
                printf("closing connection\n");
                Close(connfd);
                FD_CLR(connfd, &p->read_set);
                p->client_fds[i] = -1;
            }
        }

        /* If the descriptor is write-ready, write a line to the client */
        if ((connfd >= 0) && (FD_ISSET(connfd, &p->write_ready_set))) {
            p->nready_write--;
            if (request->is_static)
                serve_static(connfd, );
            else
                serve_dynamic(connfd);
        }
    }
}

int handle_request_errors(int connfd, request_t *request) {
    /* handle request errors */
    struct stat sbuf;
    if (stat(request->filename, &sbuf) < 0) {
        clienterror(connfd, request->filename, "404", "Not found",
            "Tiny couldn't find this file");
        return CLIENT_ERROR;
    }                                                    

    if (request->is_static) { 
        if (0 != strcasecmp(request->method, "GET")) {
            clienterror(connfd, request->method, "405", "Method Not Allowed",
                    "This method is not supported by the target resource");
            return CLIENT_ERROR;
        }
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) { 
            clienterror(connfd, request->filename, "403", "Forbidden",
                "Access denied.");
            return CLIENT_ERROR;
        }
    } else { /* request for dynamic content */
        if (0 != strcasecmp(request->method, "POST")) {
            clienterror(connfd, request->method, "405", "Method Not Allowed",
                    "This method is not supported by the target resource");
            return CLIENT_ERROR;
        }

        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) { 
            clienterror(connfd, request->filename, "403", "Forbidden",
                "Tiny couldn't run the CGI program");
            return CLIENT_ERROR;
        }

        /* only accept application/x-www-form-urlencoded */
        char *content_type = find_header(request, "Content-Type");
        if (0 != strcasecmp(content_type, "application/x-www-form-urlencoded")) {
            clienterror(connfd, content_type, "415", "Unsupported Content Type",
                "This server only accepts POST requests with Content-Type: application/x-www-form-urlencoded");
            return CLIENT_ERROR;
        }

        /* require Content-Length */
        char *content_length = find_header(request, "Content-Length");
        if (content_length == NULL) {
            clienterror(connfd, content_length, "411", "Length Required",
                "This server requires POST requests to contain a Content-Length header");
            return CLIENT_ERROR;
        }
    }
    return SUCCESS;
}


int parse_request_line(int connfd, request_t *request, char *line)
{
    printf("parse_request_line => %s", line);

    /* Parse request line if not already done */
    if (!request->request_line_done) {
        sscanf(line, "%s %s %s", request->method, request->uri, request->version);
        request->request_line_done = true;

        if (0 != strcasecmp(request->method, "GET") && 
            0 != strcasecmp(request->method, "HEAD") &&  
            0 != strcasecmp(request->method, "POST")) 
        {
            clienterror(connfd, request->method, "501", "Not Implemented",
                        "Tiny does not implement this method");
            return CLIENT_ERROR;
        }

        /* Parse URI from request */
        request->is_static = parse_uri(request->uri, request->filename, request->query_string);

        if (handle_request_errors(connfd, request) == CLIENT_ERROR)
            return CLIENT_ERROR;

    } 
    else if (!request->headers_done)
        return parse_request_header(request, line);
    else if (!request->body_done)
        return parse_request_body(connfd, request, line);

    return SUCCESS;
}

char *find_header(request_t *request, const char* name) {
    for (int i=0, count=request->header_count; i<count; i++) {
        if (0 == strcasecmp(name, request->headers[i].name))
            return request->headers[i].value;
    }
    return NULL;
}

/* NOTE(shaw): this is not robust header parsing, the server will crash if
 * headers are not formatted correctly */
int parse_request_header(request_t *request, char *line)
{
    http_header_t *headers = request->headers;
    char *p = request->headers_buffer;

    if (0 != strcmp(line, "\r\n")) {
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
    } else {
        request->headers_done = true;
    }
    return SUCCESS;
}

int parse_request_body(int connfd, request_t *request, char *line) {
    char *str_content_length = find_header(request, "Content-Length");
    int content_length = atoi(str_content_length) + 1;
    int bytes_read = strlen(request->body);
    int bytes_remaining = content_length - bytes_read;

    strncpy(request->body, line, bytes_remaining);

    if (bytes_read == content_length) {
        request->body_done = true;
    } else if (bytes_read > content_length) {
        fprintf(stderr, "Expected to read a max of %d bytes from the request body, but read %d\n", 
                content_length, bytes_read);
        clienterror(connfd, "Request Body", "500", 
                "Failed to parse the request body", 
                "Failed to parse the request body");
        return CLIENT_ERROR;
    }

    return SUCCESS;
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
        return RESPONSE_COMPLETE;

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


/* SIGCHLD handler */
void reap_children(int signum) {
    pid_t cpid;
    do {
        cpid = wait(NULL);
    } while(cpid != -1 && errno != ECHILD);
}

#if 0

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
#endif

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

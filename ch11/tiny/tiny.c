/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the 
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh 
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
#include "csapp.h"

#define MAXHEADERS 64
#define MAX_REQUEST_BODY_BYTES 2048
#define MAX_HEADERS_BYTES 2048

typedef struct {
    char *name;
    char *value;
} http_header_t;

typedef struct {
    int is_static;
    int header_count;
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], query_string[MAXLINE];
    http_header_t headers[MAXHEADERS];
    char headers_buffer[MAX_HEADERS_BYTES];
    char body[MAX_REQUEST_BODY_BYTES];
} request_t;

void doit(int fd, request_t *request);
void read_requesthdrs(rio_t *rp, request_t *request);
int parse_uri(char *uri, char *filename, char *query_string);
void serve_static(int fd, char *method, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *method, char *filename, char *query_string);
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg);
void reap_children(int signum);
void reset_request(request_t *request);
char *find_header(request_t *request, const char* name);

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    request_t *request = Malloc(sizeof(request_t));

    Signal(SIGCHLD, reap_children);

    listenfd = Open_listenfd(argv[1]);

    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); 
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        reset_request(request);
        doit(connfd, request);
        Close(connfd);
    }

    Free(request);
}


/* $end tinymain */

/*
 * doit - handle one HTTP request/response transaction
 */
/* $begin doit */
void doit(int fd, request_t *request) 
{
    struct stat sbuf;
    char buf[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", request->method, request->uri, request->version);

    if (0 != strcasecmp(request->method, "GET") && 
        0 != strcasecmp(request->method, "HEAD") &&  
        0 != strcasecmp(request->method, "POST")) 
    {
        clienterror(fd, request->method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }

    read_requesthdrs(&rio, request);

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
        Rio_readlineb(&rio, buf, atoi(content_length)+1);
        strcpy(request->body, buf);

        serve_dynamic(fd, request->method, request->filename, request->body);
    }
}
/* $end doit */
char *find_header(request_t *request, const char* name) {
    for (int i=0, count=request->header_count; i<count; i++) {
        if (0 == strcasecmp(name, request->headers[i].name))
            return request->headers[i].value;
    }
    return NULL;
}

void reset_request(request_t *request) {
    request->is_static = 0;
    request->header_count = 0;
    memset(request->headers, 0, MAXHEADERS * sizeof(http_header_t));
}


/*
 * read_requesthdrs - read HTTP request headers
 */
/* $begin read_requesthdrs */
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
/* $end read_requesthdrs */

/*
 * parse_uri - parse URI into filename and CGI args
 *             return 0 if dynamic content, 1 if static
 */
/* $begin parse_uri */
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
/* $end parse_uri */

/*
 * serve_static - copy a file back to the client 
 */
/* $begin serve_static */
void serve_static(int fd, char *method, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);    
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n", filesize);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype);
    Rio_writen(fd, buf, strlen(buf));

    if (!strcasecmp(method, "HEAD"))
        return;

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0); 
    srcp = (char *)Malloc(filesize);
    Rio_readn(srcfd, srcp, filesize);
    Close(srcfd);
    Rio_writen(fd, srcp, filesize);
    Free(srcp);
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
/* $begin serve_dynamic */
void serve_dynamic(int fd, char *method, char *filename, char *query_string) 
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
  
    if (Fork() == 0) { /* Child */
        /* Real server would set all CGI vars here */
        setenv("METHOD", method, 1);
        setenv("QUERY_STRING", query_string, 1); 
        Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */ 
        Execve(filename, emptylist, environ); /* Run CGI program */ 
    }
}
/* $end serve_dynamic */

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */

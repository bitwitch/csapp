/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
    char *query_string, *p, *method;
    char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
    int n1=0, n2=0;

    /* Extract the two arguments */
    if ((query_string = getenv("QUERY_STRING")) != NULL) {
        p = strchr(query_string, '&');
        *p = '\0';
        strcpy(arg1, query_string);
        strcpy(arg2, p+1);
        p = strchr(arg1, '=');
        n1 = p == NULL ? atoi(arg1) : atoi(p+1);
        p = strchr(arg2, '=');
        n2 = p == NULL ? atoi(arg2) : atoi(p+1);
    }

    /* Make the response body */
    sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>", 
	    content, n1, n2, n1 + n2);
    sprintf(content, "%sThanks for visiting!\r\n", content);
  
    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");

    // skip content if its a HEAD request
    method = getenv("METHOD");
    if (strcasecmp(method, "HEAD"))
        printf("%s", content);

    fflush(stdout);

    exit(0);
}
/* $end adder */

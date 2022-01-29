#include <netdb.h>
#include <stdio.h>
#include "csapp.h"

/*
struct hostent {
    char  *h_name;		 // official name of host 
    char **h_aliases;	 // alias list 
    int    h_addrtype;	 // host address type 
    int    h_length; 	 // length of address
    char **h_addr_list;	 // list of addresses
}
*/

sem_t mutex;

struct hostent *gethostbyname_ts(const char *name, struct hostent *host, 
                                 char *buf, size_t buf_size, char **list_buf, size_t list_buf_size)
{
    P(&mutex);
    struct hostent *p = gethostbyname(name);

    size_t buf_size_needed = strlen(p->h_name) + 1;
    size_t list_buf_size_needed = 0;
    int i;

    for (i=0; p->h_aliases[i]; ++i) {
        buf_size_needed += strlen(p->h_aliases[i]) + 1;
        list_buf_size_needed += sizeof(char**);
    }
    list_buf_size_needed += sizeof(char**); /* for terminating NULL pointer */

    for (i=0; p->h_addr_list[i]; ++i) {
        buf_size_needed += strlen(p->h_addr_list[i]) + 1;
        list_buf_size_needed += sizeof(char**);
    }
    list_buf_size_needed += sizeof(char**); /* for terminating NULL pointer */

    if (buf_size < buf_size_needed) {
        fprintf(stderr, "Buffer passed to gethostbyname_ts is not big enough to hold the results, needed %lu bytes\n", buf_size_needed);
        host = NULL;
    }

    if (list_buf_size < list_buf_size_needed) {
        fprintf(stderr, "List buffer passed to gethostbyname_ts is not big enough to hold the results, needed %lu bytes\n", buf_size_needed);
        host = NULL;
    }

    if (host) {
        host->h_addrtype = p->h_addrtype;
        host->h_length = p->h_length;
        host->h_aliases = list_buf;

        int i;
        char *bufp = buf;
        char **list_bufp = list_buf;

        strcpy(bufp, p->h_name);
        host->h_name = bufp;
        bufp += strlen(p->h_name) + 1;

        for (i=0; p->h_aliases[i]; ++i) {
            strcpy(bufp, p->h_aliases[i]);
            *list_bufp = bufp;
            bufp += strlen(p->h_aliases[i]) + 1;
            ++list_bufp;
        }
        *list_bufp = NULL;
        ++list_bufp;

        host->h_addr_list = list_bufp;

        for (i=0; p->h_addr_list[i]; ++i) {
            strcpy(bufp, p->h_addr_list[i]);
            *list_bufp = bufp;
            bufp += strlen(p->h_addr_list[i]) + 1;
            ++list_bufp;
        }
        *list_bufp = NULL;
        ++list_bufp;
    }

    V(&mutex);
    return host;
}


void print_hostent(struct hostent *p) {
    printf("h_name: %s\n", p->h_name);
    printf("h_aliases:\n");
    for (int i=0; p->h_aliases[i]; ++i) 
        printf("    %s\n", p->h_aliases[i]);
    printf("h_addrtype: %d\n", p->h_addrtype);
    printf("h_length: %d\n", p->h_length);
    for (int i=0; p->h_addr_list[i]; ++i) 
        printf("    %s\n", inet_ntoa( *(struct in_addr*)p->h_addr_list[i] ));

}


int main(void) {
    Sem_init(&mutex, 0, 1);

    struct hostent host;
    char buf[4096];
    char *list_buf[64];

    gethostbyname_ts("www.homestarrunner.com", &host, buf, sizeof(buf), list_buf, sizeof(list_buf));

    print_hostent(&host);
    
    return 0;
}





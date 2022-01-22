#include "csapp.h"
void *thread(void *vargp);

int main() 
{
    pthread_t tid;

    Pthread_create(&tid, NULL, thread, NULL);
    pthread_exit(NULL);


    /* calling exit in any thread in a process terminates all threads in that
     * process */
    /* calling pthread_exit instead allows any running threads to continue
     * execution until they terminate */
    /*exit(0);*/ 
}

void *thread(void *vargp) 
{
    Sleep(1);
    printf("Hello, world!\n"); 
    return NULL;
}

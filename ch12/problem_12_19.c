
int readcnt;
sem_t mutex, w; /* initally both 1 */
sem_t writer_inactive; /* initally 1 */

void reader(void) {
    while (1) {
        P(&mutex);
        readcnt++;
        V(&r);
        if (readcnt == 1) /* first in */
            P(&w);
        V(&mutex);

        /* critical section */
        /* reading happens */

        P(&mutex);
        readcnt--;
        P(&r);
        if (readcnt == 0) /* last out */
            V(&w);
        V(&mutex);
    }
}

void writer(void) {
    while (1) {
        P(&writer_inactive);
        P(&w);

        /* critical section */
        /* writing happens */


        V(&w);
        V(&writer_inactive);
    }

}


/*
These practice problems are from the SIMD Web:Aside from CSAPP3e
found here: http://csapp.cs.cmu.edu/3e/waside/waside-simd.pdf
*/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "vec.h"

/* Number of bytes in a vector */
#define VBYTES 32

/* Number of elements in a vector */
#define VSIZE VBYTES/sizeof(data_t)

/* Vector data type */
typedef data_t vec_t __attribute__ ((vector_size(VBYTES)));

#define OP +
#define IDENT 0

/* Problem 1 */
void simd_combine(vec_ptr v, data_t *dest) {
    long i;
    vec_t accum0, accum1, accum2, accum3;
    data_t *data = get_vec_start(v);
    int cnt = vec_length(v);
    data_t result = IDENT;

    /* Initialize all accum entries to IDENT */
    for (i = 0; i < VSIZE; i++) {
        accum0[i] = IDENT;
        accum1[i] = IDENT;
        accum2[i] = IDENT;
        accum3[i] = IDENT;
    }

    /* Single step until have memory alignment */
    while ((((size_t) data) % VBYTES) != 0 && cnt) {
        result = result OP *data++;
        cnt--;
    }

    /* Step through data with 4*VSIZE-way parallelism */
    while (cnt >= VSIZE*4) {
        vec_t chunk0 = *((vec_t *) data);
        accum0 = accum0 OP chunk0;

        vec_t chunk1 = *((vec_t *) (data+VSIZE));
        accum1 = accum1 OP chunk1;

        vec_t chunk2 = *((vec_t *) (data+2*VSIZE));
        accum2 = accum2 OP chunk2;

        vec_t chunk3 = *((vec_t *) (data+3*VSIZE));
        accum3 = accum3 OP chunk3;

        data += 4*VSIZE;
        cnt -= 4*VSIZE;
    }

    /* Single-step through remaining elements */
    while (cnt) {
        result = result OP *data++;
        cnt--;
    }

    /* Combine accumulators */
    vec_t accum = accum0 OP (accum1 OP (accum2 OP accum3));

    /* Combine elements of accumulator vector */
    for (i = 0; i < VSIZE; i++)
        result = result OP accum[i];

    /* Store result */
    *dest = result;
}


void scalar_combine(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);
    long limit = length - 4;
    data_t *data = get_vec_start(v);
    data_t acc = IDENT;

    // unroll loop by factor of 5
    for (i=0; i<limit; i+=5) {
        acc = acc OP data[i];
        acc = acc OP data[i+1];
        acc = acc OP data[i+2];
        acc = acc OP data[i+3];
        acc = acc OP data[i+4];
    }

    for (; i<length; i++) {
        acc = acc OP data[i];
    }

    *dest = acc;
}




int main() {
    long len = 69;
    vec_ptr v = new_vec(len);
    for (int i=0; i<len; i++)
        set_vec_element(v, i, i+1);

    data_t vresult;
    simd_combine(v, &vresult);

    data_t sresult;
    scalar_combine(v, &sresult);

    assert(vresult == sresult);

    printf("Success\n");

    return 0;
}


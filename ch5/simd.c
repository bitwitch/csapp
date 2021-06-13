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

/* Problem 2 */
void simd_combine2(vec_ptr v, data_t *dest) {
    long i;
    vec_t accum;
    data_t *data = get_vec_start(v);
    int cnt = vec_length(v);
    data_t result = IDENT;

    /* Initialize all accum entries to IDENT */
    for (i = 0; i < VSIZE; i++) {
        accum[i] = IDENT;
    }

    /* Single step until have memory alignment */
    while ((((size_t) data) % VBYTES) != 0 && cnt) {
        result = result OP *data++;
        cnt--;
    }

    /* Step through data with 4*VSIZE-way parallelism */
    while (cnt >= VSIZE*4) {
        vec_t chunk0 = *((vec_t *) data);
        vec_t chunk1 = *((vec_t *) (data+VSIZE));
        vec_t chunk2 = *((vec_t *) (data+2*VSIZE));
        vec_t chunk3 = *((vec_t *) (data+3*VSIZE));

        accum = accum OP (chunk0 OP (chunk1 OP (chunk2 OP chunk3)));

        data += 4*VSIZE;
        cnt -= 4*VSIZE;
    }

    /* Single-step through remaining elements */
    while (cnt) {
        result = result OP *data++;
        cnt--;
    }

    /* Combine elements of accumulator vector */
    for (i = 0; i < VSIZE; i++)
        result = result OP accum[i];

    /* Store result */
    *dest = result;
}


/* Problem 3 */
void vinner(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    vec_t accum;
    data_t result = 0;

    /* Initialize all accum entries to 0 */
    for (i = 0; i < VSIZE; i++) {
        accum[i] = 0;
    }

    /* Single step until have memory alignment */
    while ((((size_t) udata) % VBYTES) != 0 && length) {
        result = result + (*udata * *vdata);
        udata++;
        vdata++;
        length--;
    }

    /* Step through data with VSIZE-way parallelism */
    while (length >= VSIZE) {
        vec_t uchunk = *((vec_t *) udata);
        vec_t vchunk = *((vec_t *) vdata);
        accum = accum + (uchunk * vchunk);

        udata += VSIZE;
        vdata += VSIZE;
        length -= VSIZE;
    }

    /* Single-step through remaining elements */
    while (length) {
        result = result + (*udata * *vdata);
        udata++;
        vdata++;
        length--;
    }

    /* Combine elements of accumulator vector */
    for (i = 0; i < VSIZE; i++)
         result += accum[i];

    /* Store result */
    *dest = result;
}

void inner(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t result = 0;

    while (length) {
        result = result + *udata * *vdata;
        udata++;
        vdata++;
        length--;
    }

    *dest = result;
}


/* Problem 4 */
void vinner2(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    vec_t accum0, accum1, accum2, accum3;
    data_t result = 0;

    /* Initialize all accum entries to 0 */
    for (i = 0; i < VSIZE; i++) {
        accum0[i] = 0;
        accum1[i] = 0;
        accum2[i] = 0;
        accum3[i] = 0;
    }

    /* Single step until have memory alignment */
    while ((((size_t) udata) % VBYTES) != 0 && length) {
        result = result + (*udata * *vdata);
        udata++;
        vdata++;
        length--;
    }

    /* Step through data with 4*VSIZE-way parallelism */
    while (length >= 4*VSIZE) {
        vec_t uchunk0 = *((vec_t *) udata);
        vec_t vchunk0 = *((vec_t *) vdata);
        accum0 += uchunk0 * vchunk0;

        vec_t uchunk1 = *((vec_t *) (udata+VSIZE));
        vec_t vchunk1 = *((vec_t *) (vdata+VSIZE));
        accum1 += uchunk1 * vchunk1;

        vec_t uchunk2 = *((vec_t *) (udata+VSIZE*2));
        vec_t vchunk2 = *((vec_t *) (vdata+VSIZE*2));
        accum2 += uchunk2 * vchunk2;

        vec_t uchunk3 = *((vec_t *) (udata+VSIZE*3));
        vec_t vchunk3 = *((vec_t *) (vdata+VSIZE*3));
        accum3 += uchunk3 * vchunk3;

        udata += 4*VSIZE; 
        vdata += 4*VSIZE;
        length -= 4*VSIZE;
    }

    /* Single-step through remaining elements */
    while (length) {
        result = result + (*udata * *vdata);
        udata++;
        vdata++;
        length--;
    }

    /* Combine vector accumulators */
    vec_t accum = accum0 + (accum1 + (accum2 + accum3));

    /* Combine elements of accumulator vector */
    for (i = 0; i < VSIZE; i++)
         result += accum[i];

    /* Store result */
    *dest = result;
}


/* Problem 5 */
double vpoly(double a[], double x, long degree) {
    long i;
    double result = a[0];
    double xpwr = x;
    vec_t accum;
    vec_t vxpwr;
    vec_t vx;

    // start at a[1]
    a++;
    long len = degree;

    // ensure alignment to VBYTES
    while ((((size_t) a) % VBYTES) != 0 && len >= VSIZE) {
        result += *a * xpwr;
        xpwr *= x;
        a++;
        len--;
    }

    double x_pow_vsize = 1;
    for (i = 0; i < VSIZE; i++ ) {
        x_pow_vsize *= x;
    }

    // generate vector of xpwr and a vector to 'increment' xpwrs on
    // each iteration of the main loop
    for (i = 0; i < VSIZE; i++ ) {
        vxpwr[i] = xpwr;
        xpwr *= x;

        vx[i] = x_pow_vsize;
    }

    // combine with VSIZE parallelism
    while (len >= VSIZE) {
        vec_t chunk = *((vec_t *) a);
        accum += chunk * vxpwr;
        vxpwr *= vx;
        a += VSIZE;
        len -= VSIZE;
    }

    // get last power of x
    xpwr = vxpwr[0];

    // single step remaining elements
    while (len) {
        result += *a * xpwr;
        xpwr *= x;
        a++;
        len--;
    }

    // combine accumulator
    for (i=0; i<VSIZE; i++)
        result += accum[i];

    return result;
}

double poly(double a[], double x, long degree) {
    long i;
    double result = a[0]; 
    double xpwr = x;
    for (i = 1; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    return result;
}




int main() {
    /* Problem 1 */
    long len = 69;
    vec_ptr v = new_vec(len);
    for (int i=0; i<len; i++)
        set_vec_element(v, i, i+1);

    data_t vresult;
    simd_combine(v, &vresult);

    data_t sresult;
    scalar_combine(v, &sresult);

    assert(vresult == sresult);

    /* Problem 2 */
    data_t v2result;
    simd_combine2(v, &v2result);
    
    assert(v2result == sresult);

    /* Problem 3 */
    vec_ptr u = new_vec(len);
    for (int i=0; i<len; i++)
        set_vec_element(u, i, i*.5);

    data_t vinner_result;
    vinner(u, v, &vinner_result);

    data_t inner_result;
    inner(u, v, &inner_result);

    assert(vinner_result == inner_result);

    /* Problem 4 */
    data_t vinner2_result;
    vinner2(u, v, &vinner2_result);

    assert(vinner2_result == vinner_result);

    /* Problem 5 */
    double a[len];
    for (int i=0; i<len; i++)
        a[i] = i+1;

    double x = 3.14;

    long degree = 9;

    double poly_result = poly(a, x, degree);
    double vpoly_result = vpoly(a, x, degree);

    printf("vpoly: %f\n poly: %f\n", vpoly_result, poly_result);
    assert(vpoly_result == poly_result);


    printf("Success\n");

    return 0;
}


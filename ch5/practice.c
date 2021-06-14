#include <stdio.h>

/* 5.5 */
double poly(double a[], double x, long degree) {
    long i;
    double result = a[0]; 
    double xpwr = x;
    for (i = 1; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr *= x;
    }
    return result;
}


/* 5.6 */
double polyh(double a[], double x, long degree) {
    long i;
    double result = a[degree];
    for (i = degree-1; i >= 0; i--) {
        result = a[i] + x * result;
    }
    return result;
}


/* 5.7 */
#include "vec.h"

#define IDENT 1
#define OP *

void combine5(vec_ptr v, data_t *dest) {
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

/* 5.8 */
double aprod(double a[], long n) {
    long i;
    double x, y, z;
    double r = 1;
    for (i=0; i<n-2; i+=3) {
        x = a[i]; y = a[i+1]; z = a[i+2];
        /*r = ((r * x) * y) * z; // A1 */
        /*r = (r * (x * y)) * z; // A2 */
        /*r = r * ((x * y) * z); // A3 */
        /*r = r * (x * (y * z)); // A4 */
        r = (r * x) * (y * z); // A5 
    }
    for (; i<n; i++)
        r *= a[i];
    return r;
}

/* 5.9 */
void merge(long src1[], long src2[], long dest[], long n) {
    long i1 = 0;
    long i2 = 0;
    long id = 0;

    while (i1 < n && i2 < n) {
        /*if (src1[i1] < src2[i2]) {*/
            /*dest[id] = src1[i1];*/
            /*i1++;*/
        /*} else {*/
            /*dest[id] = src2[i2];*/
            /*i2++;*/
        /*}*/
        /*id++;*/

        bool v1_is_min = src1[i1] < src2[i2];
        long min_val = v1_is_min ? src1[i1] : src2[i2];
        long *min_idx = v1_is_min ? &i1 : &i2;
        *min_idx = *min_idx + 1;
        dest[id++] = min_val;
    }
    while (i1 < n)
        dest[id++] = src1[i1++];
    while (i2 < n)
        dest[id++] = src2[i2++];
}

/* 5.12 */
void psum1(float a[], float p[], long n) {
    long i;
    p[0] = a[0];
    for (i=1; i<n; i++)
        p[i] = p[i-1] + a[i];
}



int main() {
    double a[6] = {3, 1, 4, 1, 5, 9};
    double x = 3.14;
    long degree = 5;

    double poly_result = poly(a, x, degree);
    double polyh_result = polyh(a, x, degree);

    printf("poly result: %f\npolyh result: %f\n", poly_result, polyh_result);

    long len = 69;

    vec_ptr v = new_vec(len);
    for (int i=0; i<len; i++)
        set_vec_element(v, i, i+1);

    data_t combine5_result;
    combine5(v, &combine5_result);

    printf("combine5 result: %f\n", combine5_result); 

    long src1[len];
    long src2[len];
    long merged[len*2];

    for (long i=0; i<len; i++) {
        src1[i] = 2 * i;
        src2[i] = 2 * i + 1;
     }

    merge(src1, src2, merged, len);

    int j = 0;
    for (long i=0; i<len*2; i++, j++) {
        if (j > 9) {
            printf("\n");
            j = 0;
        }
        printf("%3ld ", merged[i]);
    }
    printf("\n");



    float p[len];
    psum1(src1, p, len);

    j = 0;
    for (long i=0; i<len; i++, j++) {
        if (j > 9) {
            printf("\n");
            j = 0;
        }
        printf("%3ld ", p[i]);
    }
    printf("\n");



    return 0;
}


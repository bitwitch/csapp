#include <assert.h>
#include <stdio.h>
#include "vec.h"

/* 5.13

    A.

    rcx       xmm0    
     |          |        
     |----------|-------load
     |          |         |
     |----------|---load  |
     |          |     |   |
     |          |     ---mul
     |          |         |
     |          ---------add
     |                    | 
     |          ----------- 
     |          |         
    add         |         
     |          |         
    rcx       xmm0   

    The critical path is the float add, stored in xmm0 at each iteration


    B. 
    Floating point addition has a latency of 3 cycles, so the lower bound on 
    CPE is 3

    C. 
    Integer addition has a latency of 1 cycle, so the lower bound on CPE is 1

    D.
    The multiplication does not have data dependencies between iterations, so
    it does not limit the CPE.
*/

/* 5.14 */
void inner_6x1(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;

    for (i=0; i+6<length; i+=6) {
        sum += udata[i] * vdata[i];
        sum += udata[i+1] * vdata[i+1];
        sum += udata[i+2] * vdata[i+2];
        sum += udata[i+3] * vdata[i+3];
        sum += udata[i+4] * vdata[i+4];
        sum += udata[i+5] * vdata[i+5];
    }

    for (; i<length; i++) 
        sum += udata[i] * vdata[i];

    *dest = sum;
}
/*
    A.
    The unrolled loop has 6 add operations, and unrolling the loop leaves n/6
    iterations for vector inputs of length n.  So this results in a CPE of 1

    B.
    Floating point addition has a latency of 3, so the loop is bound by 6*3=18
    cycles. With n/6 iterations, this yields 18 * n/6 = 3n, so CPE is 3.
*/

/* 5.15 */
void inner_6x6(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum0 = (data_t) 0;
    data_t sum1 = sum0;
    data_t sum2 = sum0;
    data_t sum3 = sum0;
    data_t sum4 = sum0;
    data_t sum5 = sum0;

    for (i=0; i+6<length; i+=6) {
        sum0 += udata[i] * vdata[i];
        sum1 += udata[i+1] * vdata[i+1];
        sum2 += udata[i+2] * vdata[i+2];
        sum3 += udata[i+3] * vdata[i+3];
        sum4 += udata[i+4] * vdata[i+4];
        sum5 += udata[i+5] * vdata[i+5];
    }

    for (; i<length; i++) 
        sum0 += udata[i] * vdata[i];

    sum0 += sum1 + sum2 + sum3 + sum4 + sum5;

    *dest = sum0;
}

/* 5.16 */
void inner_6x6a(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;

    for (i=0; i+6<length; i+=6) {
        sum += udata[i]   * vdata[i]   +
               udata[i+1] * vdata[i+1] +
               udata[i+2] * vdata[i+2] +
               udata[i+3] * vdata[i+3] +
               udata[i+4] * vdata[i+4] +
               udata[i+5] * vdata[i+5];
    }

    for (; i<length; i++) 
        sum += udata[i] * vdata[i];

    *dest = sum;
}

/* 5.17 */
void *better_memset(void *s, int c, size_t n) {
    size_t i;

    unsigned char *schar = s;

    unsigned long clong = (unsigned long) c;

    unsigned long packed = 0;
    size_t bytes_in_long = sizeof(unsigned long);

    for (i = 0; i < bytes_in_long; i++) {
        packed |= clong << (8 * i);
    }

    // step by byte until aligned by however many bytes in long
    for (i = 0;
        i < n && ((size_t)schar) % bytes_in_long != 0;
        i++) 
    {
        *schar++ = (unsigned char) c;
    }

    unsigned long *slong = (unsigned long *)schar;

    // step by long
    for (; i + bytes_in_long <= n; i += bytes_in_long) {
        *slong++ = packed;
    }

    schar = (unsigned char *)slong;

    // step by byte to finish any remaining
    for (; i < n; i++) {
        *schar++ = (unsigned char) c;
    }

    return s;
}

/* 5.18 */
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

/*
.L2:
	cmpq	%rsi, %rax
	jg	.L4
	movapd	%xmm1, %xmm3
	mulsd	(%rdi,%rax,8), %xmm3
	addsd	%xmm3, %xmm2
	mulsd	%xmm0, %xmm1
	addq	$1, %rax
	jmp	.L2


    rax   xmm1                xmm2 
     |      |                   |
     |      +-----mov-----      |
     |      |            |      | 
     +-----------load---mul----add
     |      |                   |
     |      |                   | 
     |      |                   |
     |     mul                  |
     |      |                   |
    add     |                   |
     |      |                   |
    rax    xmm1               xmm2

            ^ 
            |__ this is the critical path

*/

double polyh(double a[], double x, long degree) {
    long i;
    double result = a[degree];
    for (i = degree-1; i >= 0; i--) {
        result = a[i] + x * result;
    }
    return result;
}

/*
.L6:
	testq	%rsi, %rsi
	js	.L8
	mulsd	%xmm0, %xmm1
	addsd	(%rdi,%rsi,8), %xmm1
	subq	$1, %rsi
	jmp	.L6


    rsi            xmm1
     |              |
    test           mul
     |              |
     +-------------add
     |              |
    sub             |
     |              |
    rax            xmm1

*/
double poly_fast(double a[], double x, long degree) {
    long i;

    // assumes degree >= 4
    double result = a[0];

    double xpwr0 = x;
    double xpwr1 = x * xpwr0;
    double xpwr2 = x * xpwr1;
    double xpwr3 = x * xpwr2;
    double xpwr4 = x * xpwr3;

    for (i = 1; i+4 <= degree; i+=5) {
        result += a[i]   * xpwr0 +
                  a[i+1] * xpwr1 +
                  a[i+2] * xpwr2 +
                  a[i+3] * xpwr3 +
                  a[i+4] * xpwr4;

        xpwr0 *= x;
        xpwr1 *= x;
        xpwr2 *= x;
        xpwr3 *= x;
        xpwr4 *= x;
    }

    for (; i <= degree; i++) {
        result += a[i] * xpwr4;
        xpwr4 *= x;
    }

    return result;
}

/* 5.19 */
void psum1(float a[], float p[], long n) {
    long i;
    p[0] = a[0];
    float sum = p[0];
    for (i=1; i<n; i++) {
        sum = sum + a[i];
        p[i] = sum;
    }
}

void psum_fast(float a[], float p[], long n) {
    long i;
    p[0] = a[0];

    float sum0, sum1, sum2, sum3;
    sum3 = a[0];

    for (i=1; i<n-3; i+=4) {
        sum0 = sum3 + a[i];
        sum1 = sum0 + a[i+1];
        sum2 = sum1 + a[i+2];
        sum3 = sum2 + a[i+3];

        p[i]   = sum0;
        p[i+1] = sum1;
        p[i+2] = sum2;
        p[i+3] = sum3;
    }

    for (; i<n; i++) {
        sum3 += a[i];
        p[i] = sum3;
    }
}



int main() {
    /* 5.14 */
    long len = 10;

    // first 10 evens
    vec_ptr u = new_vec(len);
    for (int i=0; i<len; i++)
        set_vec_element(u, i, 2*i);

    // first 10 odds
    vec_ptr v = new_vec(len);
    for (int i=0; i<len; i++)
        set_vec_element(v, i, 2*i+1);

    data_t inner_6x1_result;
    inner_6x1(u, v, &inner_6x1_result);

    assert(inner_6x1_result == (data_t) 1230);

    /* 5.15 */
    data_t inner_6x6_result;
    inner_6x6(u, v, &inner_6x6_result);

    assert(inner_6x6_result == (data_t) 1230);

    /* 5.16 */
    data_t inner_6x6a_result;
    inner_6x6a(u, v, &inner_6x6a_result);

    assert(inner_6x6a_result == (data_t) 1230);


    /* 5.17 */
    int nums[10];
    char *two_bytes_into_nums = (char *)nums + 2;
    better_memset(two_bytes_into_nums, 0x69, 8 * sizeof(int));

    char *data_byte = two_bytes_into_nums;
    for (size_t i = 0; i < 8 * sizeof(int); i++) {
        assert(*data_byte == 0x69);
        data_byte++;
    }

    /* 5.18 */
    double a[6] = {3, 1, 4, 1, 5, 9};
    double x = 3.14159;
    long degree = 5;

    double poly_result = poly(a, x, degree);
    double polyh_result = polyh(a, x, degree);
    double poly_fast_result = poly_fast(a, x, degree);

    assert(poly_result == poly_fast_result);

    /* 5.19 */
    float floats[len];
    float p[len];
    float p2[len];
    for (int i=0; i<len; i++)
        floats[i] = i * 3.14;

    psum1(floats, p, len);
    psum_fast(floats, p2, len);

    for (long i=0; i<len; i++)
        assert(p[i] == p2[i]);

    printf("Success\n");
    
    return 0;
}

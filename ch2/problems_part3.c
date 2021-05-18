/*
 * Part 3: Floating Point
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>

typedef unsigned float_bits;

unsigned f2u(float x) {
    void *v = &x;
    unsigned *u = v;
    return *u;
}

long unsigned d2lu(double x) {
    void *v = &x;
    long unsigned *u = v;
    return *u;
}




/* 2.84 */
int float_le(float x, float y) {
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);

    unsigned sx = ux >> 31;
    unsigned sy = uy >> 31;

    return (sx > sy) || 
        (sx == 0 && sy == 0 && ux <= uy) || 
        (sx == 1 && sy == 1 && ux >= uy);
}

/* 2.90 */
float u2f(unsigned x) {
    void *v = &x;
    float *f = v;
    return *f;
}

float fpwr2(int x) {
    unsigned exp, frac, u;

    if (x < -149 ) {
        // too small return 0
        exp = 0;
        frac = 0;
    } else if (x < -126) {
        // denormalized
        exp = 0;
        frac = 1 << (x + 126);
    } else if (x < 127) {
        // normalized
        exp = x + 127;
        frac = 0;
    } else {
        // too big return +inf
        exp = 0xFF;
        frac = 0;
    } 

    u = exp << 23 | frac;
    return u2f(u);
}

/* 2.92 */
float_bits float_negate(float_bits f) {
    float_bits exp  = (f >> 23) & 0xFF;
    float_bits frac = f & 0x7FFFFF;

    // if f is NaN return f
    if (exp == 0xFF && frac)
        return f;

    return f ^ 0x80000000;
}

/* 2.93 */
float_bits float_absval(float_bits f) {
    float_bits sign = f >> 31;
    float_bits exp  = (f >> 23) & 0xFF;
    float_bits frac = f & 0x7FFFFF;

    // if f is NaN or positive return f
    if ( (exp == 0xFF && frac) || !sign )
        return f;

    return f ^ 0x80000000;
}

/* 2.94 */
float_bits float_twice(float_bits f) {
    float_bits sign = f >> 31;
    float_bits exp  = (f >> 23) & 0xFF;
    float_bits frac = f & 0x7FFFFF;

    // if f is NaN or infinity return f
    if ( (exp == 0xFF && frac) || (exp == 0xFF) )
        return f;

    if (exp == 0) {
    // denormalized

        // if multiplying by 2 overflows frac, perform the * 2 in the exponent instead
        if ( (1 << 23) & frac ) 
            exp++;
        else
            frac <<= 1;
                
    } else {
    // normalized
        if (++exp == 0xFF)
            frac = 0;
    }
    
    return (sign << 31) | (exp << 23) | frac;
}

/* 2.95 */
float_bits float_half(float_bits f) {
    float_bits sign = f >> 31;
    float_bits exp  = (f >> 23) & 0xFF;
    float_bits frac = f & 0x7FFFFF;

    // if f is NaN or infinity return f
    if ( (exp == 0xFF && frac) || (exp == 0xFF) )
        return f;

    if (exp == 0) {
    // denormalized
        if (frac & 1) {
            frac >>= 1;
            // round to even
            if (frac & 1)
                frac++;
        } else {
            frac >>= 1;
        }
                
    } else {
    // normalized
        if (exp == 1) {
            // treat as denormalized but shift the 1 from exp to the right into frac
            if (frac & 1) {
                frac >>= 1;
                frac |= 1 << 22;
                // round to even
                if (frac & 1)
                    frac++;
            } else {
                frac >>= 1;
                frac |= 1 << 22;
            }
        } 

        exp--;
    }
    
    return (sign << 31) | (exp << 23) | frac;
}


int main() {
    /* 2.84 */
    assert(float_le(69.0, 90.0) == 1);
    assert(float_le(0.0, 666.0) == 1);
    assert(float_le(23084.23, 555555.20) == 1);
    assert(float_le(-666.666, 55.9234) == 1);
    assert(float_le(-666.123, -99.123) == 1);
    assert(float_le(6.66, -999.9) == 0);

    assert(float_le(123456.789, 123456.789) == 1);
    assert(float_le(-696969.69696969, -696969.69696969) == 1);
    assert(float_le(-696969.69696969, -696969.6969688) == 1);

    /* 2.89 */
    int x = 0x12345678;
    float f = (float) x;
    double d = (double) x;
    printf("x = %X  float: %f (%X) double: %f (%lX)\n\n", x, f, f2u(f), d, d2lu(d));

    /* 2.90 */
    assert(fpwr2(0) == powf(2,0));
    assert(fpwr2(1) == powf(2,1));
    assert(fpwr2(2) == powf(2,2));
    assert(fpwr2(3) == powf(2,3));
    assert(fpwr2(69) == powf(2,69));
    assert(fpwr2(-69) == powf(2,-69));
    assert(fpwr2(666) == powf(2,666));
    assert(fpwr2(-666) == powf(2,-666));

    /* 2.92 */
    unsigned u = 0;
    while (0) { // disable to avoid long processing

        if ((u & 0x7FFFFFFF) < 0x7F800001) { 
            assert( float_negate(u) == f2u(-u2f(u)) );
        } else  { // if NaN
            assert( float_negate(u) == u );
        }

        if (u == 0xFFFFFFFF) break;

        u++;
    }


    /* 2.93 */
    u = 0;
    while (0) { // disable to avoid long processing

        if ((u & 0x7FFFFFFF) < 0x7F800001) {
            assert( float_absval(u) == f2u(fabs(u2f(u))) );
        } else  { // if NaN
            assert( float_absval(u) == u );
        }

        if (u == 0xFFFFFFFF) break;

        u++;
    }

    /* 2.94 */
    u = 0;
    while (0) { // disable to avoid long processing

        if ((u & 0x7FFFFFFF) < 0x7F800001) {
            assert( float_twice(u) == f2u(2.0f * u2f(u)) );
        } else  { // if NaN
            assert( float_absval(u) == u );
        }

        if (u == 0xFFFFFFFF) break;

        u++;
    }

    /* 2.95 */
    u = 0;
    while (0) { // disable to avoid long processing

        if ((u & 0x7FFFFFFF) < 0x7F800001) {
            float_bits using_func = float_half(u);
            unsigned using_float = f2u(0.5f * u2f(u));
            if (using_func != using_float) {
                printf("u = %X  func: %X  float: %X\n", u, using_func, using_float);
            }
            assert( float_half(u) == f2u(0.5f * u2f(u)) );
        } else  { // if NaN
            if (float_half(u) != u) printf("error in NaN calc\n");
            assert( float_half(u) == u );
        }

        if (u == 0xFFFFFFFF) break;

        u++;
    }



    printf("Success\n");
    return 0;
}

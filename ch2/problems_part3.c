/*
 * Part 3: Floating Point
 */

#include <assert.h>
#include <stdio.h>

unsigned f2u(float x) {
    void *v = &x;
    unsigned *u = v;
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

    printf("Success\n");
    return 0;
}

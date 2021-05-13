#include <assert.h>
#include <stdio.h>

/* 2.13 */
int bis(int x, int m) {
    return x | m;
}

int bic(int x, int m) {
    return x & ~m;
}

int bool_or (int x, int y) {
    return bis(x, y);
}

int bool_xor (int x, int y) {
    return bis( bic(x,y), bic(y,x) );
}

/* 2.25 */
int uadd_okay(unsigned x, unsigned y) {
    return x + y > x;
}

int main() {
    int a = 0x89DEFAB;
    int b = 0x1234DCFE;

    assert(bool_or(a, b) == (a | b));
    assert(bool_xor(a, b) == (a ^ b));

    assert(uadd_okay(4000000000, 3000000000) == 0);
    assert(uadd_okay(1000000000, 1000000000) == 1);

    printf("Success\n");

    return 0;
}

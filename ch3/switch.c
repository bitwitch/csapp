#include <stdio.h>

void switch_eg(long x, long n, long *dest) {
    long val = x;

    switch(n) { 

    case 100:
        val *= 13;
        break;

    case 102:
        val += 10;
        // fall through

    case 103:
        val += 11;
        break;
        
    case 104:
    case 106:
        val *= val;
        break;

    default:
        val = 0;
    }

    *dest = val;
}

int main() {

    long dest;
    for (int i=100; i<107; i++) {
        switch_eg(1, i, &dest);
        printf("x = 1  n = %d  dest = %ld\n", i, dest);
    }
    return 0;
}

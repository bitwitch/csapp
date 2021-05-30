#include <stdio.h>
#include <inttypes.h>

/* 3.58 */
long decode2(long x, long y, long z) {
    long result = (y - z);
    long var = result * x;
    result <<= 63;
    result >>= 63;
    result ^= var;
    return result;
}

/* 3.59 */
typedef __int128 i128;
typedef int64_t i64;

void store_prod(i128 *dest, i64 x, i64 y) {
    *dest = x * (i128) y;
}
/*
dest in rdi, x in rsi, y in rdx
store_prod:
    movq %rdx, %rax        store y
    cqto                   convert to octword, y_high rdx, y_low in rax
    movq %rsi, %rcx        store x
    sarq $63, %rcx         get x_high
    imulq %rax, %rcx       multiply y_low and x_high
    imulq %rsi, %rdx       multiply y_high and x_low
    addq %rdx, %rcx        add result of multplications
    mulq %rsi              multiply y and x (full 128 bits %rdx:%rax)
    addq %rcx, %rdx        add p_high to the earlier multiplication sum
    movq %rax, %(rdi)      store low bytes of result
    movq %rdx, 8(%rdi)     store high bytes of result
    ret


    basically this is a long way to do what imulq can do in a single instruction
    it does the longform computations (FOIL) of:
    (x_high + h_low) * (y_high + y_low) 
    => (x_high * y_hight) + (x_high * y_low) + (x_low * y_high) + (x_low * y_low)

*/

/* 3.60 */
long loop(long x, int n) {
    long result = 0;
    long mask;
    for (mask = 1; mask != 0; mask = mask >> (n & 0xFF) ) {
        result |= mask & x;
    }
    return result;
}
/*
A. rdi and r8 store x, esi and ecx store n, rax stores result, rdx stores mask
B. result = 0; mask = 1
C. mask != 0
D. mask is shifted right arithmetic by the low byte of n
E. result |= mask & x
*/


/* 3.61 */
long cread(long *xp) {
    return (xp ? *xp : 0);
}

long cread_alt(long *xp) {
    long default_val = 0;
    long *addr = xp ? xp : &default_val;
    return *addr;
}

int main() {
    /* 3.58 */
    long result = decode2(7, 8, 9);
    printf("decode2(7, 8, 9) --> %ld\n", result);
    
    /* 3.61 */
    long *xp = NULL;
    long val = cread_alt(xp);
    printf("value read from cread: %ld\n", val);


    return 0;
}



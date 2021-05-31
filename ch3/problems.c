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


/* 3.62 */
typedef enum { MODE_A, MODE_B, MODE_C, MODE_D, MODE_E } mode_t;
long switch3(long *p1, long *p2, mode_t action) {
    long result = 0;
    switch(action) {
    case MODE_A:
        result = *p2;
        *p2 = *p1;
        break;
    case MODE_B:
        result = *p1 + *p2;
        *p1 = result;
        break;
    case MODE_C:
        *p1 = 59;
        result = *p2;
        break;
    case MODE_D:
        *p1 = *p2;
    case MODE_E:
        result = 27;
        break;
    default:
        result = 12;
        break;
    }
    return result;
}


/* 3.63 */
long switch_prob(long x, long n) {
    long result = x;
    switch(n) {
    case 60:
    case 62:
        result = x + 8;
        break;
    case 63:
        result = x >> 3;
        break;
    case 64:
        result = x << 4;
        result -= x;
        result *= result;
        result += 75;
        break;
    default:
        result = x + 75;
        break;
    }
    return result;
}


/* 3.64 */
#define R 7
#define S 5
#define T 13

long A[R][S][T];
long store_ele(long i, long j, long k, long *dest) {
    *dest = A[i][j][k];
    return sizeof(A);
}

/*
-----------------------------------------------
                    A.
-----------------------------------------------
[[[1,2,3],
  [4,5,6],
  [7,8,9]],

 [[1,2,3],
  [4,5,6],
  [7,8,9]]
 
 [[1,2,3],
  [4,5,6],
  [7,8,9]]]

[1,2,3, 4,5,6, 7,8,9, 1,2,3, 4,5,6, 7,8,9 ...

D[1][1][2] = 6

(i * B * C) + (C * j + k)

... 

for D[S][T]
&D[j][k] = x_d + L * (T * j + k)

for D[R][S][T]
&D[i][j][k] = x_d + L * ( (i * S * T) + (T * j + k) )


-----------------------------------------------
                   B.
-----------------------------------------------
rax = 13 * j
rsi = i * 64
rdi = 65 * i + 13 * j
rdx = 65 * i + 13 * j + k

65*i + 13*j + k = i*S*T + T*j + k

T = 13
S = 5

sizeof(A) = 3640 = 8 * R * S * T
R = 3640 / (8 * 13 * 5)

R = 7
S = 5
T = 13

*/


/* 3.65 */
#define M 15
void transpose(long A[M][M]) {
    long i, j;
    for (i = 0; i < M; i++)
        for (j = 0; j < i; j++) {
            long t = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = t;
        }
}
/*

   1 2 3 
   4 5 6      
   7 8 9

   1 4 7
   2 5 8      
   3 6 9



A. rdx holds a pointer to A[i][j]
B. rax holds a pointer to A[j][i]
C. M = 15

*/

void print_array(long A[M][M]) {
    int j,i;
    for (j=0; j<M; j++)
        for (i=0; i<M; i++) {
            printf("%ld ", A[j][i]);
            if (i == M - 1)
                printf("\n");
        }
}


/* 3.66 */
#define NR(n) 3 * n
#define NC(n) 4 * n + 1

long sum_col(long n, long A[NR(n)][NC(n)], long j) {
    long i;
    long result = 0;
    for (i=0; i<NR(n); i++)
        result += A[i][j];
    return result;
}

/*
r8 = 1 + 4n
rax = 3n
rdi = 3n

r8 = 8 * (1 + 4n)


NR(n) = 3n
NC(n) = 1 + 4n

*/

/* 3.67 */
typedef struct {
        long a[2];
        long *p;
} strA;

typedef struct {
        long u[2];
        long q;
} strB;

strB process(strA s) {
    strB r;
    r.u[0] = s.a[1];
    r.u[1] = s.a[0];
    r.q = *s.p;
    return r;
}

long eval(long x, long y, long z) {
    strA s;
    s.a[0] = x;
    s.a[1] = y;
    s.p = &z;
    strB r = process(s);
    return r.u[0] + r.u[1] + r.q;
}

/*
A. 
   |            .             |
   |            .             |
   |            .             |
   |                          |
   |--------------------------|
   |            z             |
   |--------------------------|<--- rsp + 24
   |      &z (rsp + 24)       |
   |--------------------------|<--- rsp + 16
   |            y             |
   |--------------------------|<--- rsp + 8
   |            x             |
    -------------------------- <--- rsp
          top of stack

This shows the stack before calling process.
Once process is called, the return address will also be pushed on the stack.

B. It passes rsp + 64, the address of strB r that process will return

C. 
   |            .             |
   |            .             |
   |            .             |
   |                          |
   |--------------------------|
   |            z             |
   |--------------------------|<--- rsp + 32
   |      &z (rsp + 32)       |
   |--------------------------|<--- rsp + 24
   |            y             |
   |--------------------------|<--- rsp + 16
   |            x             |
   |--------------------------|<--- rsp + 8
   |       return addr        |
    -------------------------- <--- rsp
          top of stack

'process' accesses the elements of s using the stack pointer and offsets.

D. The process function uses the argument passed in rdi by eval. This value 
is an address on the stack. It uses rdi and offsets to set fields of r.

E. 
   |            .             |
   |            .             |
   |            .             |
   |--------------------------|
   |            z             |
   |--------------------------|<--- rsp + 80
   |            x             |
   |--------------------------|<--- rsp + 72
   |            y             |
   |--------------------------|<--- rsp + 64
   |            .             |     start of r
   |            .             |
   |            .             |
   |--------------------------|
   |            z             |
   |--------------------------|<--- rsp + 24
   |      &z (rsp + 32)       |
   |--------------------------|<--- rsp + 16
   |            y             |
   |--------------------------|<--- rsp + 8
   |            x             |
    -------------------------- <--- rsp
          top of stack

F. A structure can be passed as an argument on the stack.

Memory for a structure returned from a callee can be allocated on the stack by
the caller. The address of the structure's location can be passed as an
argument in a register to the callee.
*/

/* 3.68 */
#define A 10
#define B 8

typedef struct {
    int x[A][B];
    long y;
} str1;

typedef struct {
    char array[B];
    int t;
    short s[A];
    long u;
} str2;

void set_val(str1 *p, str2 *q) {
    long v1 = q->t;
    long v2 = q->u;
    p->y = v1+v2;
}

/*
   |--------------------------|
   |            u             |
   |--------------------------|<--- rsi + 32
   |                          |
   |                          |
   |           s[10]          |
   |                          |
   |                          |
   |--------------------------|<--- rsi + 12
   |            t             |
   |--------------------------|<--- rsi + 8
   |         array[8]         |
    -------------------------- <--- rsi


   |--------------------------|
   |            y             |
   |--------------------------|<--- rdi + 184
   |          empty           |
   |--------------------------|<--- rdi + 180
   |                          |
   |                          |
   |         x[10][8]         |
   |                          |
   |                          |
    -------------------------- <--- rdi

A = 10
B = 8

*/

/* 3.69 */
#define CNT 7

typedef struct {
    long idx;
    long x[4];
} a_struct;
/* 40 bytes */

typedef struct {
    int first;
    a_struct a[CNT];
    int last;
} b_struct

void test(long i, b_struct *bp) {
    int n = bp->first + bp->last;
    a_struct *ap = &bp->a[i];
    ap->x[ap->idx] = n;
}
/*
ecx = n

rax = bp + 40i
rdx = 8 + (bp + 40i) 
rcx = n

dest_addr = 16 + (bp + 40i) + 8 * (8 + (bp + 40i))


int        <-- 288

array[7]   <-- 8

empty      

int        <-- 0

CNT = 7

*/



int main() {
    /* 3.58 */
    long result = decode2(7, 8, 9);
    printf("decode2(7, 8, 9) --> %ld\n", result);
    
    /* 3.61 */
    long *xp = NULL;
    long val = cread_alt(xp);
    printf("value read from cread: %ld\n", val);
    
    /* 3.62 */
    long p1 = 6, p2 = 9;
    result = switch3(&p1, &p2, MODE_D);
    printf("switch3 result = %ld\n", result);

    /* 3.65 */
    long A[M][M] = {
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14}
    };
    printf("Before:\n");
    print_array(A);
    transpose(A);
    printf("After:\n");
    print_array(A);
    


    return 0;
}



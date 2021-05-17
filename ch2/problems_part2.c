#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> // for memcpy


/* 2.61 */
int any_bit_1(int x) {
    return x || 0;
}

int any_bit_0(int x) {
    return (x ^ ~0) || 0;
}

int any_bit_in_lsb_1(int x) {
    char lsb = (char)(x & 0xFF);
    return lsb || 0;
}

int any_bit_in_msb_0(int x) {
    int shift = (sizeof(x) - 1) << 3;
    /* get most significant byte */
    char msb = (char)(x >> shift);
    return (msb ^ ~0) || 0;
}


/* 2.62 */
int int_shifts_are_arithmetic() {
    int shift = (sizeof(int) - 1) << 3;
    int high_byte_ones = 0xFF << shift;
    int test = high_byte_ones >> 8;
    return ((test >> shift) & 0xFF) || 0;
}

/* 2.63 */
unsigned srl(unsigned x, int k) {
    // number of bits in int
    int w = sizeof(int) << 3;

    if (k >= w) return x;

    // perform shift arithmetically
    unsigned xsra = (int) x >> k;

    // how many ones needed in the mask
    int ones = w - k;
    int mask = (1 << ones) - 1;

    return xsra & mask;
}

int sra(int x, int k) {
    // number of bits in int
    int w = sizeof(int) << 3;

    if (k >= w) return x;

    // perform shift logically
    int xsrl = (unsigned) x >> k;

    int high_bit = (unsigned) x >> (w - 1);

    // The high_bit is either 1 or 0.
    // In the case of a 1, mask is k ones shifted left by w - k.
    // In the case of a 0, mask is 0.
    int mask = ((1 << k) - high_bit) << (w - k);

    return xsrl | mask;
}

/* 2.64 */
// return 1 when any odd bit of x equals 1; 0 otherwise. assume w=32 
int any_odd_one(unsigned x) {
    return (0xAAAAAAAA & x) || 0;
}

/* 2.65  HARD! */
// return 1 when x contains an odd number of 1s; 0 otherwise. assume w=32
int odd_ones(unsigned x) {
//    0000 0101 1111 1000   
//    0x05F8









// at most 12 arithmetic, bitwise, and logical operations

// 0000    0
// 0001    1
// 0010    2
// 0011    3
// 0100    4
// 0101    5
// 0110    6
// 0111    7
// 1000    8
// 1001    9
// 1010   10
// 1011   11
// 1100   12
// 1101   13
// 1110   14
// 1111   15


    return 0;

}

/* 2.66 */
// Generate a mask indicating leftmost 1 in x. assume w=32
// Use at most 15 arithmetic, bitwise, and logical operations
int leftmost_one(unsigned x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    
    return x ^ (x >> 1);
}

/* 2.67 */
/*
int bad_int_size_is_32() {
    // set msb of 32 bit machine
    int set_msb = 1 << 31;

    // set past msb of 32 bit machine
    int beyond_msb = 1 << 32;

    // set_msb is nonzero when word size >= 32
    // beyond_msb is zero when word size <=32
    return set_msb && !beyond_msb;


    // A.
    // The C standard only guarantees a minimum of 16 bit integers. It also
    // does not standardize behaviour for bit shifts >= the bit length of an
    // integer. Therefore shifts of 31 and 32 bits are undefined behaviour
    // according to the standard.
    
}
*/
// B. Modify for any machine for which int is at least 32 bits
int int_size_is_32() {
    int set_msb = 1 << 31;
    return !(set_msb << 1);
}

// C. Modify for any machine for which int is at least 16 bits
int any_int_size_is_32() {
    int set_msb = 1 << 15;

    int is_16 = !(set_msb << 1);

    // set bit 31 to one on 32-bit machine or all zeros on < 32-bit
    set_msb <<= 15;
    set_msb <<= 1;

    return !is_16 && !(set_msb << 1);
}

/* 2.68 */
// mask with least sgnificant n bits set to 1
// n = 6 --> 0x3F, n = 17 --> 0x1FFFF
// assume 1 <= n <= w
int lower_one_mask(int n) {

    // would be nice to do (1 << n) - 1, but doesn't work for n = w
     

    int mask = 1 << (n - 1);
    mask |= mask >> 1;
    mask |= mask >> 2;
    mask |= mask >> 4;
    mask |= mask >> 8;
    // for full c standard compliance could >> 15 then >> 1
    mask |= mask >> 16;

    return mask;
}

/* 2.69 */
// do rotating left shift. assume 0 <= n < w
unsigned rotate_left(unsigned x, int n) {
    int w = sizeof(int) << 3;

    // shift the n high bits down to the low n bits
    int escaped_bits = x >> (w - n);

    // perform regular left shift by n
    x <<= n;

    // put the escaped bits in the low n bits
    return x | escaped_bits;
}

/* 2.70 */
// return 1 when x can be represented as an n-bit, 2's-compliment number;
// 0 otherwise. asume 1 <= n <= w
int fits_bits(int x, int n) {
    // mask off the bits greater than n
    int mask = lower_one_mask(n);
    int n_bit = x & mask;

    // non negative
    int non_neg_will_fit = (x == n_bit);

    // negative
    // the n - 1 bit must be 1
    // and all bits n and greater must be 1
    int high_bits_all_one = (~mask & x) == ~mask;
    int n_minus_one_bit = 1 << (n - 1);

    int neg_will_fit = high_bits_all_one && 
                       ((n_minus_one_bit & n_bit) == n_minus_one_bit);


    return non_neg_will_fit || neg_will_fit;
}

/* 2.71 */
typedef unsigned packed_t;

int buggy_xbyte(packed_t word, int bytenum) {
    return (word >> (bytenum << 3)) & 0xFF;
}

// A. This implementation fails to sign extend the number.

//B. use only left and right shifts, and one subtraction
// Extract byte from word, return as signed integer
int xbyte(packed_t word, int bytenum) {
    return (int) (word << ((3 - bytenum) << 3)) >> 24;
}

/* 2.72 */
void buggy_copy_int(int val, void *buf, int maxbytes) {
    if (maxbytes - sizeof(val) >= 0)
        printf("copying\n");
        memcpy(buf, (void *) &val, sizeof(val));
}

// A. Since size_t is an unsigned type, maxbytes gets implicitly converted to
// an unsigned. When the subtraction takes place, if maxbytes is less than
// sizeof(val), it will underflow into a large positive number. It will always
// be >= 0.

// B.
void copy_int(int val, void *buf, int maxbytes) {
    if (maxbytes - (int)sizeof(val) >= 0)
        printf("copying\n");
        memcpy(buf, (void *) &val, sizeof(val));
}

/* 2.73 */
// addition that saturates to TMin or Tmax
int saturating_add(int x, int y) {
    int w = sizeof(int) << 3;

    int sum = x + y;

    int msb = 1 << (w - 1); // most sig bit

    // HANDLE OVERFLOW
    int high_bit = msb & (~x) & (~y) & sum;

    int mask = high_bit >> (w - 1);

    // if overflow, mask will be all ones, otherwise all zeros
    // after applying the mask, set the most sig bit based on high_bit, this
    // handles the overflow case setting the value to int max
    sum = (sum | mask) ^ high_bit;

    // HANDLE UNDERFLOW
    high_bit = msb & x & y & ~sum;

    mask = ~(high_bit >> (w - 1));

    // if underflow, mask will will be all zeros, otherwise all ones
    // after applying the mask, set the most sig bit based on high_bit, this
    // handles the underflow case setting the value to int min
    sum = (sum & mask) | high_bit;

    return sum;
}

int simplified_saturating_add(int x, int y) {
    int msb = INT_MIN;
    int sum = x + y;

    int overflow = (msb & ~x & ~y) && (msb & sum);
    int underflow = (msb & x & y) && !(msb & sum);

    (overflow && (sum = INT_MAX)) || (underflow && (sum = INT_MIN));

    return sum;
}

/* 2.74 */
// determine wheter arguments can be subtracted without overflow
int tsub_okay(int x, int y) {
    int mask = INT_MIN;
    int diff = x - y;

    // if x is pos and y is neg, and diff is neg - overflow
    int pos_overflow = (~x & mask) && (y & mask) && (diff & mask);

    // if x is neg and y is pos, and diff is pos - overflow
    int neg_overflow = (x & mask) && (~y & mask) && (~diff & mask);

    return !(pos_overflow || neg_overflow);
}

/* 2.75 */
int signed_high_prod(int x, int y) {
    int64_t prod = (int64_t) x * y;
    return prod >> 32;
}

unsigned unsigned_high_prod(unsigned x, unsigned y) {
    int mask = INT_MIN;

    int y_contrib = 0;
    int x_contrib = 0;

    (x & mask) && (y_contrib = y);
    (y & mask) && (x_contrib = x);

    int high_prod = signed_high_prod(x, y);

    return high_prod + y_contrib + x_contrib;
}

/* 2.76 */
void *calloc(size_t nmemb, size_t size) {
    if (nmemb == 0 || size == 0)
        return NULL;

    size_t total_size = nmemb * size;

    // if overflow return null
    if (total_size < nmemb || total_size < size)
        return NULL;

    void *memory = malloc(total_size);
    memset(memory, 0, total_size);

    return memory;
}

/* 2.77 */
void problem_2_77(int x) {
    int prod;

    // A. K = 17
    prod = (x << 4) + x;
    assert(x * 17 == prod);

    // B. K = -7
    prod = x - (x << 3);
    assert(x * -7 == prod);

    // C. K = 60
    prod = (x << 6) - (x << 2);
    assert(x * 60 == prod);

    // D. K = -112
    prod = (x << 4) - (x << 7);
    assert(x * -112 == prod);
}

/* 2.78 */
// divide by power of 2. assume 0 <= k < w-1
int divide_power2(int x, int k) {
    int q = x >> k;

    // for negative x
    (x & INT_MIN) && (q = ((x + (1 << k) - 1) >> k));

    return q;
}

/* 2.79 */
int mul3div4(int x) {
   // mul 3
   int prod = (x << 2) - x;
   // div 4
   return divide_power2(prod, 2);
}

/* 2.80 */
int threefourths(int x) {

    int w = sizeof(int) << 3;

    int prod = (x << 2) - x;

    int mask = INT_MIN | (INT_MIN >> 1);



    // store the bits shifted out of range
    // most significant 2 bits
    int ms2b = x & mask;
    
    // determine effect on these bits of subtracting x 
    // considering the 4 high bits (0-3), if bit 1 (from left) is 0 and bit 3 is 1
    // then the 2 high bits will be affected

    /*
     * Somehow the the subtraction doesn't affect the high bits that get shifted out,
     * I don't really understand this. For example in the case where x is INT_MIN, when
     * you subtract x from (x << 2), you would need to borrow from the higher order bits
     * which would affect them, but the math does not work out when you do this. The high
     * bits just get put back in no matter what, unchanged. 
     *
    !(((unsigned) INT_MIN >> 2) & x) && 
        (INT_MIN & x) && 
        (ms2b = ((ms2b >> (w - 2)) - 1) << (w - 2));
    */

    // perform division
    int result = divide_power2(prod, 2);

    // set the high bits 
    result &= ~mask;
    result |= ms2b;

    return result;
}

/* 2.81 */
void problem_2_81(int j, int k) {
    // A.  (w - k) ones followed by k zeros
    int A = ~((1 << k) - 1);
     
    // B.  (w - k - j) zeros, then k ones, then j zeros
    int B = ((1 << k) - 1) << j;

    printf("j = %d, k = %d\nA = %X\nB = %X\n\n", j, k, A, B);
}




int main() {

    /* 2.61 */
    printf("any bit 1\n");
    printf("0xFF --> %d\n", any_bit_1(0xFF));
    printf("0x01 --> %d\n", any_bit_1(0x01));
    printf("0x00 --> %d\n", any_bit_1(0x00));
    printf("\n");

    printf("any bit 0\n");
    printf("0x00       --> %d\n", any_bit_0(0x00));
    printf("0x01       --> %d\n", any_bit_0(0x01));
    printf("0xFFFFFFFF --> %d\n", any_bit_0(0xFFFFFFFF));
    printf("\n");

    printf("any bit in lsb 1\n");
    printf("0x87654300 --> %d\n", any_bit_in_lsb_1(0x87654300));
    printf("0x87654321 --> %d\n", any_bit_in_lsb_1(0x87654321));
    printf("\n");

    printf("any bit in msb 0\n");
    printf("0x87654321 --> %d\n", any_bit_in_msb_0(0x87654321));
    printf("0xFF654321 --> %d\n", any_bit_in_msb_0(0xFF654321));
    printf("\n");

    /* 2.62 */
    printf("int shifts are arithmetic: %d\n", int_shifts_are_arithmetic());
    printf("\n");

    /* 2.63 */
    printf("srl(0xFF000000, 8) --> %.8X\n", srl(0xFF000000, 8));
    printf("srl(0x0F000000, 8) --> %.8X\n", srl(0x0F000000, 8));
    printf("\n");

    printf("sra(0xFF000000, 8) --> %.8X\n", sra(0xFF000000, 8));
    printf("sra(0x0F000000, 8) --> %.8X\n", sra(0x0F000000, 8));
    printf("\n");

    /* 2.64 */
    printf("any odd one 0xAAAAAAAA --> %d\n", any_odd_one(0xAAAAAAAA));
    printf("any odd one 0xA0000000 --> %d\n", any_odd_one(0xA0000000));
    printf("any odd one 0x55555555 --> %d\n", any_odd_one(0x55555555));
    printf("\n");

    /* 2.66 */
    assert(leftmost_one(0xFF00) == 0x8000);
    assert(leftmost_one(0x6600) == 0x4000);
    assert(leftmost_one(0x0) == 0x0);
    assert(leftmost_one(0x80000000) == 0x80000000);

    /* 2.67 */
    assert(int_size_is_32());
    assert(any_int_size_is_32());

    /* 2.68 */
    assert(lower_one_mask(6) == 0x3F);
    assert(lower_one_mask(17) == 0x1FFFF);
    assert(lower_one_mask(32) == 0xFFFFFFFF);

    /* 2.69 */
    assert(rotate_left(0x12345678, 4) == 0x23456781);
    assert(rotate_left(0x12345678, 20) == 0x67812345);

    /* 2.70 */
    assert(fits_bits(0xFF, 8) == 1);
    assert(fits_bits(0xFFF, 8) == 0);
    assert(fits_bits(0xFFFF, 16) == 1);
    assert(fits_bits(0xFFFFF, 16) == 0);
    assert(fits_bits(0xFFFFFF, 24) == 1);
    assert(fits_bits(0xFFFFFFF, 24) == 0);
    assert(fits_bits(0xFFFFFFFF, 32) == 1);

    assert(fits_bits(0xFFFFFF88, 8) == 1);
    assert(fits_bits(0xFFFFFF64, 8) == 0);
    assert(fits_bits(0xFFFF8000, 16) == 1);
    assert(fits_bits(0xFFF80000, 16) == 0);

    /* 2.71 */
    assert(xbyte(0xABCDEF12, 2) == 0xFFFFFFCD);
    assert(xbyte(0xFF000000, 3) == 0xFFFFFFFF);
    assert(xbyte(0x12345678, 3) == 0x12);
    assert(xbyte(0x12345678, 2) == 0x34);

    /* 2.72 */
    char buf[2]; // 2 bytes
    // buggy_copy_int(69, buf, 2);
    copy_int(69, buf, 2);

    /* 2.73 */
    assert(saturating_add(0x7, 0x8) == 0xF);
    assert(saturating_add(0x7FFFFFFF, 0x7FFFFABC) == 0x7FFFFFFF);
    assert(saturating_add(0xFFFFFE17, 0x4DD) == 0x2F4);
    assert(saturating_add(0xFFFFFE5C, 0xFFFFFFBB) == 0xFFFFFE17);
    assert(saturating_add(0x80000000, 0xFFFFFFFF) == 0x80000000);
    assert(simplified_saturating_add(0x7, 0x8) == 0xF);
    assert(simplified_saturating_add(0x7FFFFFFF, 0x7FFFFABC) == 0x7FFFFFFF);
    assert(simplified_saturating_add(0xFFFFFE17, 0x4DD) == 0x2F4);
    assert(simplified_saturating_add(0xFFFFFE5C, 0xFFFFFFBB) == 0xFFFFFE17);
    assert(simplified_saturating_add(0x80000000, 0xFFFFFFFF) == 0x80000000);


    /* 2.74 */
    assert(tsub_okay(0x12345678, 0x0DCBA876) == 1);
    assert(tsub_okay(0x12345678, 0xF8000000) == 1);
    assert(tsub_okay(0x72345678, 0xC0000000) == 0);
    assert(tsub_okay(0xC0000000, 0xFFF123BC) == 1);
    assert(tsub_okay(0xFF000000, 0x0069ABBA) == 1);
    assert(tsub_okay(0xC0000000, 0x72345678) == 0);

    /* 2.75 */
    assert(unsigned_high_prod(0x4CC90E3, 0x69) == 0x1);
    assert(unsigned_high_prod(0x7CC90E3, 0xABBA) == 0x53B);
    assert(unsigned_high_prod(0x7CC90E3, 0xC0FFEE) == 0x5E138);

    /* 2.76 */
    void *p1 = calloc(69, sizeof(int));
    printf("p1 = %p\n\n", p1);
    assert(p1 != NULL);
    assert(calloc(0, sizeof(int)) == NULL);
    assert(calloc(666, 0) == NULL);
    assert(calloc(0xFFFFFFFFFFFFFFFF, 0x69) == NULL);

    /* 2.77 */
    problem_2_77(69);

    /* 2.78 */
    assert(divide_power2(6969, 8) == 27);
    assert(divide_power2(666420, 7) == 5206);
    assert(divide_power2(-666, 3) == -83);
    assert(divide_power2(-58008, 8) == -226);

    /* 2.79 */
    assert(mul3div4(0x7FFFFFFF) == (int) 0x7FFFFFFF * 3 / 4);
    assert(mul3div4(0x12345678) == (int) 0x12345678 * 3 / 4);
    assert(mul3div4(0xC0FFEE) == (int) 0xC0FFEE * 3 / 4);
    assert(mul3div4(0xFFFFABBA) == (int) 0xFFFFABBA * 3 / 4);

    /* 2.80 */
    assert(threefourths(0x1) == 0x0);
    assert(threefourths(0x4) == 0x3);
    assert(threefourths(0x9) == 0x6);
    assert(threefourths(0xFFFFFFBB) == 0xFFFFFFCD);
    assert(threefourths(0xFFFFFC88) == 0xFFFFFD66);
    assert(threefourths(0x7FFFFFFF) == 0x5FFFFFFF);
    assert(threefourths(0x80000000) == 0xA0000000);
    assert(threefourths(0x80000003) == 0xA0000003);

    /* 2.81 */
    problem_2_81(6, 9);
    problem_2_81(4, 13);
    problem_2_81(22, 3);



    printf("Success\n");

    return 0;
}

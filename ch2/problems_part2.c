#include <stdio.h> 
#include <assert.h>

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


int main() {

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

    printf("int shifts are arithmetic: %d\n", int_shifts_are_arithmetic());
    printf("\n");


    printf("srl(0xFF000000, 8) --> %.8X\n", srl(0xFF000000, 8));
    printf("srl(0x0F000000, 8) --> %.8X\n", srl(0x0F000000, 8));
    printf("\n");

    printf("sra(0xFF000000, 8) --> %.8X\n", sra(0xFF000000, 8));
    printf("sra(0x0F000000, 8) --> %.8X\n", sra(0x0F000000, 8));
    printf("\n");

    printf("any odd one 0xAAAAAAAA --> %d\n", any_odd_one(0xAAAAAAAA));
    printf("any odd one 0xA0000000 --> %d\n", any_odd_one(0xA0000000));
    printf("any odd one 0x55555555 --> %d\n", any_odd_one(0x55555555));
    printf("\n");

    assert(leftmost_one(0xFF00) == 0x8000);
    assert(leftmost_one(0x6600) == 0x4000);
    assert(leftmost_one(0x0) == 0x0);
    assert(leftmost_one(0x80000000) == 0x80000000);


    assert(int_size_is_32());
    assert(any_int_size_is_32());

    assert(lower_one_mask(6) == 0x3F);
    assert(lower_one_mask(17) == 0x1FFFF);
    assert(lower_one_mask(32) == 0xFFFFFFFF);

    assert(rotate_left(0x12345678, 4) == 0x23456781);
    assert(rotate_left(0x12345678, 20) == 0x67812345);

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

    return 0;
}

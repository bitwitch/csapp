/*
Chapter 2 problems 2.55 - 2.60 from Computer Systems: A Programmer's 
Perspective Third Edition (page 128)
*/

#include <stdio.h>

typedef unsigned char *byte_pointer;

/* 2.55 - 2.56 */
void show_bytes(byte_pointer start, size_t len) {
    for (int i=0; i<len; i++) {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(void *));
}

/* 2.57 */
void show_short(short x) {
    show_bytes((byte_pointer) &x, sizeof(short));
}

void show_long(long x) {
    show_bytes((byte_pointer) &x, sizeof(long));
}

void show_double(double x) {
    show_bytes((byte_pointer) &x, sizeof(double));
}

/* 2.58 */
int is_little_endian() {
    int one = 1;
    byte_pointer first_byte = (byte_pointer) &one;
    return *first_byte == 1;
}

/* 2.59 */
int least_x_remaining_y(int x, int y) {
    return (x & 0xFF) | (y & ~0xFF);
}

/* 2.60 */
unsigned replace_byte(unsigned x, int i, unsigned char b) {
    if (i < 0 || i >= sizeof(unsigned)) return x;
    unsigned result = x & ~(0xFF << (i * 8));
    result |= (unsigned)b << (i * 8);
    return result;
}

int main() {

    printf("7   :"); show_int(7);
    printf("69  :"); show_int(69);
    printf("666 :"); show_int(666);
    printf("\n");

    printf("-7   :"); show_int(-7);
    printf("-69  :"); show_int(-69);
    printf("-666 :"); show_int(-666);
    printf("\n");

    printf("7.2    :"); show_float(7.2);
    printf("69.666 :"); show_float(69.666);
    printf("-4.20  :"); show_float(-4.20);
    printf("\n");

    printf("22  :"); show_short(22);
    printf("3000000000     :"); show_long(3000000000);
    printf("30000000000.1  :"); show_double(30000000000.1);
    printf("\n");

    printf("is little endian: %d\n\n", is_little_endian());

    int x = 0x89ABCDEF;
    int y = 0x76543210;
    printf("x: %X y: %X result: %X\n\n", x, y, least_x_remaining_y(x, y));


    printf("replace_byte(0x12345678, 2, 0xAB) --> %X\n", replace_byte(0x12345678, 2, 0xAB));
    printf("replace_byte(0x12345678, 0, 0xAB) --> %X\n", replace_byte(0x12345678, 0, 0xAB));
    printf("\n");

    return 0;
}

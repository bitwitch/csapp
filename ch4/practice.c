#include <stdio.h>

/* Practice Problem 4.4 */
long rsum(long *start, long count) {
    if (count <= 0)
        return 0;
    return *start + rsum(start+1, count-1);
}

/*
Y86-64 Code:
start in %rdi, count in %rsi

rsum:
    xorq %rax, %rax       // result = 0
    andq %rsi, %rsi       // set CC
    jle done

    pushq %rdi            // save this stack frame

    irmovq $8, %r8
    irmovq $1, %r9
    addq %r8, %rdi        // rdi = start+1
    subq %r9, %rsi        // rsi = count-1

    call rsum

    popq %rdi             // restore stack frame

    mrmovq (%rdi), %r8
    addq %r8, %rax

done:
    ret;
*/


/*
Practice Problem 4.5

long abs_sum(long *start, long count)
start in rdi, count in rsi

Y86-64 Code:

abs_sum: 
    irmovq $8, %r8
    irmovq $1, %r9
    xorq %rax, %rax
    andq %rsi, %rsi
    jmp test
loop: 
    mrmovq (%rdi), %r11
    andq %r11, %11
    jge pos
    xorq %r10, %r10
    subq %r11, %r10
    rrmovq %r10, %r11
pos:
    addq %r11, %rax
    addq %r8, %rdi
    subq %r9, %rsi
test:
    jne loop
    ret
*/


/*
Practice Problem 4.6

long abs_sum(long *start, long count)
start in rdi, count in rsi

Y86-64 Code:

abs_sum: 
    irmovq $8, %r8
    irmovq $1, %r9
    xorq %rax, %rax
    andq %rsi, %rsi
    jmp test
loop: 
    mrmovq (%rdi), %r11
    xorq %r10, %r10
    subq %r11, %r10
    cmovg %r10, %r11
    addq %r11, %rax
    addq %r8, %rdi
    subq %r9, %rsi
test:
    jne loop
    ret
*/





int main() {

    long nums[8] = { 3, 5, 6, 12, 13, 66, 68, 8 };
    long count = 8;
    long sum = rsum(nums, count);
    printf("sum = %ld\n", sum);

    return 0;
}

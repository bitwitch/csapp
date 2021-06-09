#include <stdio.h>

/* 4.45
A. No it does not, it decrements rsp before pushing it on the stack.

ex: 
rsp = 100
subq $8, %rsp         rsp = 92
movq %rsp, (%rsp)     puts 92 on the stack

B. 
movq %rsp, %rdx
subq $8, %rsp         
movq %rdx, (%rsp)    
*/


/* 4.46 
A. No it does not, it moves the value on the stack into rsp and then increments it.

ex:
rsp = 100
top of stack = 69

movq (%rsp), %rsp       rsp = 69
addq $8, %rsp           rsp = 77

B. 
movq (%rsp), %rdx
addq $8, %rsp
movq %rdx, %rsp
*/


/* 4.47 */
void bubble_a(long *data, long count) {
    long i, last;
    for (last = count-1; last > 0; last--) {
        for (i = 0; i < last; i++) {
            if (data[i+1] < data[i]) {
                long t = data[i+1];
                data[i+1] = data[i];
                data[i] = t;
            }
        }
    }
}

/* A. */
void bubble_p(long *data, long count) {
    long i, last;
    for (last = count-1; last > 0; last--) {
        for (i = 0; i < last; i++) {
            long *curr = data+i;
            long *next = data+i+1;
            if (*next < *curr) {
                long t = *next;
                *next = *curr;
                *curr = t;
            }
        }
    }
}

void print_array(long *arr, long count) {
    for (int i=0; i<count; i++)
        printf("%ld ", arr[i]);
    printf("\n");
}


/* 
B. 
Y86-64 Program written elsewhere, check sim/y86-code

*/


/* 4.51 

Stage          iaddq V, rB
---------------------------------
Fetch          icode:ifun <-- M[PC]  == C:0
               rA:rB <-- M[PC+1] == F:rB
               valC <-- M[PC+2]
               valP <-- PC+10

Decode         valB <-- R[rB]

Execute        valE <-- valC + valB
               set CC

Memory 

Write back     r[rB] <-- valE

PC update      PC <-- valP

*/






int main() {
    long nums[9] = {6,2,3,5,9,1,4,8,7};
    long count = 9;

    printf("Before:\n");
    print_array(nums, count);

    bubble_a(nums, count);

    printf("After:\n");
    print_array(nums, count);

    return 0;
}

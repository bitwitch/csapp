#include <stdio.h>

long rfact(long n) {
    long result;
    if (n <= 1) 
        result = 1;
    else
        result = n * rfact(n-1);
    return result;
}

int main() {
    long value = rfact(5);
    printf("%ld", value);
    return 0;
}

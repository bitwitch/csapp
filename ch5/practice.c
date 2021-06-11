#include <stdio.h>

/* 5.5 */
double poly(double a[], double x, long degree) {
    long i;
    double result = a[0];
    double xpwr = x;
    for (i = 1; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    return result;
}


/* 5.6 */
double polyh(double a[], double x, long degree) {
    long i;
    double result = a[degree];
    for (i = degree-1; i >= 0; i--) {
        result = a[i] + x * result;
    }
    return result;
}


int main() {
    double a[6] = {3, 1, 4, 1, 5, 9};
    double x = 3.14;
    long degree = 5;

    double poly_result = poly(a, x, degree);
    double polyh_result = polyh(a, x, degree);

    printf("poly result: %f\npolyh result: %f\n", poly_result, polyh_result);


    return 0;
}


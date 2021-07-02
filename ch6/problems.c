#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void transpose(int *dst, int *src, long dim) {
    long i=0, j, k, l;
    int stride = 8;

    for (i=0; i<dim-(stride-1); i+=stride) {
        for (j=0; j<dim-(stride-1); j+=stride) {

            for (l=0; l<stride; l++) {
                for (k=0; k<stride; k++) {
                    dst[(j+k)*dim+i+l] = src[(i+l)*dim+j+k];
                }
            }

        }
    }

    long remaining = j;

    for (i=0; i<remaining; i++)
        for (j=remaining; j<dim; j++)
            dst[j*dim+i] = src[i*dim+j];

    for (; i<dim; i++)
        for (j=0; j<dim; j++)
            dst[j*dim+i] = src[i*dim+j];

}

void print_matrix(int *m, long dim) {
    long i, j;
    for (i=0; i<dim; i++) {
        for (j=0; j<dim; j++)
            printf("%3d ", m[i*dim+j]);
        printf("\n");
    }
    printf("\n");
}

void init_matrix(int *m, long dim) {
    long i, j, n = 0;
    for (i=0; i<dim; i++) {
        for (j=0; j<dim; j++) {
            m[i*dim+j] = n++;
        }
    }
}

int check_transpose(int *m1, int *m2, long dim) {
    long i, j;
    for (i=0; i<dim; i++)
        for (j=0; j<dim; j++)
            if (m1[j*dim+i] != m2[i*dim+j])
                return 0;
    return 1;
}




#define SIZE1 10
#define SIZE2 34080

int main() {

    int m1[SIZE1][SIZE1];
    int m2[SIZE1][SIZE1];
    init_matrix(m1, SIZE1);

    printf("Before:\n");
    print_matrix(m1, SIZE1);

    transpose(m2, m1, SIZE1);

    printf("After:\n");
    print_matrix(m2, SIZE1);

    assert(check_transpose(m1,m2,SIZE1));

    int *m3 = malloc((long)SIZE2 * (long)SIZE2 * sizeof(int));
    if (m3 == NULL) {
        printf("malloc failed for m3\n");
        return 1;
    }
    int *m4 = malloc((long)SIZE2 * (long)SIZE2 * sizeof(int));
    if (m4 == NULL) {
        printf("malloc failed for m4\n");
        return 1;
    }

    printf("size of matrix in bytes: %ld\n", (long)SIZE2 * (long)SIZE2 * sizeof(int));

    init_matrix(m3, SIZE2);

    transpose(m4, m3, SIZE2);

    assert(check_transpose(m3,m4,SIZE2));

    free(m3);
    free(m4);

    return 0;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void transpose(int *dst, int *src, long dim) {
    long i, j;
    int stride = 8;
    for (i=0; i<dim-7; i+=stride) {
        for (j=0; j<dim-7; j+=stride) {

            dst[(j+0)*dim+i] = src[i*dim+j+0];
            dst[(j+1)*dim+i] = src[i*dim+j+1];
            dst[(j+2)*dim+i] = src[i*dim+j+2];
            dst[(j+3)*dim+i] = src[i*dim+j+3];
            dst[(j+4)*dim+i] = src[i*dim+j+4];
            dst[(j+5)*dim+i] = src[i*dim+j+5];
            dst[(j+6)*dim+i] = src[i*dim+j+6];
            dst[(j+7)*dim+i] = src[i*dim+j+7];

            dst[(j+0)*dim+i+1] = src[(i+1)*dim+j+0];
            dst[(j+1)*dim+i+1] = src[(i+1)*dim+j+1];
            dst[(j+2)*dim+i+1] = src[(i+1)*dim+j+2];
            dst[(j+3)*dim+i+1] = src[(i+1)*dim+j+3];
            dst[(j+4)*dim+i+1] = src[(i+1)*dim+j+4];
            dst[(j+5)*dim+i+1] = src[(i+1)*dim+j+5];
            dst[(j+6)*dim+i+1] = src[(i+1)*dim+j+6];
            dst[(j+7)*dim+i+1] = src[(i+1)*dim+j+7];

            dst[(j+0)*dim+i+2] = src[(i+2)*dim+j+0];
            dst[(j+1)*dim+i+2] = src[(i+2)*dim+j+1];
            dst[(j+2)*dim+i+2] = src[(i+2)*dim+j+2];
            dst[(j+3)*dim+i+2] = src[(i+2)*dim+j+3];
            dst[(j+4)*dim+i+2] = src[(i+2)*dim+j+4];
            dst[(j+5)*dim+i+2] = src[(i+2)*dim+j+5];
            dst[(j+6)*dim+i+2] = src[(i+2)*dim+j+6];
            dst[(j+7)*dim+i+2] = src[(i+2)*dim+j+7];

            dst[(j+0)*dim+i+3] = src[(i+3)*dim+j+0];
            dst[(j+1)*dim+i+3] = src[(i+3)*dim+j+1];
            dst[(j+2)*dim+i+3] = src[(i+3)*dim+j+2];
            dst[(j+3)*dim+i+3] = src[(i+3)*dim+j+3];
            dst[(j+4)*dim+i+3] = src[(i+3)*dim+j+4];
            dst[(j+5)*dim+i+3] = src[(i+3)*dim+j+5];
            dst[(j+6)*dim+i+3] = src[(i+3)*dim+j+6];
            dst[(j+7)*dim+i+3] = src[(i+3)*dim+j+7];

            dst[(j+0)*dim+i+4] = src[(i+4)*dim+j+0];
            dst[(j+1)*dim+i+4] = src[(i+4)*dim+j+1];
            dst[(j+2)*dim+i+4] = src[(i+4)*dim+j+2];
            dst[(j+3)*dim+i+4] = src[(i+4)*dim+j+3];
            dst[(j+4)*dim+i+4] = src[(i+4)*dim+j+4];
            dst[(j+5)*dim+i+4] = src[(i+4)*dim+j+5];
            dst[(j+6)*dim+i+4] = src[(i+4)*dim+j+6];
            dst[(j+7)*dim+i+4] = src[(i+4)*dim+j+7];

            dst[(j+0)*dim+i+5] = src[(i+5)*dim+j+0];
            dst[(j+1)*dim+i+5] = src[(i+5)*dim+j+1];
            dst[(j+2)*dim+i+5] = src[(i+5)*dim+j+2];
            dst[(j+3)*dim+i+5] = src[(i+5)*dim+j+3];
            dst[(j+4)*dim+i+5] = src[(i+5)*dim+j+4];
            dst[(j+5)*dim+i+5] = src[(i+5)*dim+j+5];
            dst[(j+6)*dim+i+5] = src[(i+5)*dim+j+6];
            dst[(j+7)*dim+i+5] = src[(i+5)*dim+j+7];

            dst[(j+0)*dim+i+6] = src[(i+6)*dim+j+0];
            dst[(j+1)*dim+i+6] = src[(i+6)*dim+j+1];
            dst[(j+2)*dim+i+6] = src[(i+6)*dim+j+2];
            dst[(j+3)*dim+i+6] = src[(i+6)*dim+j+3];
            dst[(j+4)*dim+i+6] = src[(i+6)*dim+j+4];
            dst[(j+5)*dim+i+6] = src[(i+6)*dim+j+5];
            dst[(j+6)*dim+i+6] = src[(i+6)*dim+j+6];
            dst[(j+7)*dim+i+6] = src[(i+6)*dim+j+7];

            dst[(j+0)*dim+i+7] = src[(i+7)*dim+j+0];
            dst[(j+1)*dim+i+7] = src[(i+7)*dim+j+1];
            dst[(j+2)*dim+i+7] = src[(i+7)*dim+j+2];
            dst[(j+3)*dim+i+7] = src[(i+7)*dim+j+3];
            dst[(j+4)*dim+i+7] = src[(i+7)*dim+j+4];
            dst[(j+5)*dim+i+7] = src[(i+7)*dim+j+5];
            dst[(j+6)*dim+i+7] = src[(i+7)*dim+j+6];
            dst[(j+7)*dim+i+7] = src[(i+7)*dim+j+7];
        }
    }
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



#define SIZE1 5
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

    free(m3);
    free(m4);

    return 0;
}

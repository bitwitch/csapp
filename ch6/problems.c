#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 6.45 */
void slow_transpose(int *dst, int *src, long dim) {
    int i,j;
    for (i=0; i<dim; i++)
        for (j=0; j<dim; j++)
            dst[j*dim+i] = src[i*dim+j];
}

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


/* 6.46 */
void slow_graph_convert(int *g, int dim) {
    int i, j;
    for (i=0; i<dim; i++)
        for (j=0; j<dim; j++)
            g[j*dim+i] = g[j*dim+i] || g[i*dim+j];
}

void graph_convert(int *g, long dim) {
    long i=0, j, k, l;
    int c0, c1, c2, c3;
    int stride = 4;
    int max_stride = dim - (stride-1);

    for (i=0; i<max_stride; i+=stride) {
        for (j=0; j<max_stride; j+=stride) {
            c0 = g[(j+0)*dim+i+0];
            c1 = g[(j+0)*dim+i+1];
            c2 = g[(j+0)*dim+i+2];
            c3 = g[(j+0)*dim+i+3];

            g[(j+0)*dim+i+0] = c0 || g[(i+0)*dim+j+0];
            g[(j+0)*dim+i+1] = c1 || g[(i+1)*dim+j+0];
            g[(j+0)*dim+i+2] = c2 || g[(i+2)*dim+j+0];
            g[(j+0)*dim+i+3] = c3 || g[(i+3)*dim+j+0];

            c0 = g[(j+1)*dim+i+0];
            c1 = g[(j+1)*dim+i+1];
            c2 = g[(j+1)*dim+i+2];
            c3 = g[(j+1)*dim+i+3];

            g[(j+1)*dim+i+0] = c0 || g[(i+0)*dim+j+1];
            g[(j+1)*dim+i+1] = c1 || g[(i+1)*dim+j+1];
            g[(j+1)*dim+i+2] = c2 || g[(i+2)*dim+j+1];
            g[(j+1)*dim+i+3] = c3 || g[(i+3)*dim+j+1];

            c0 = g[(j+2)*dim+i+0];
            c1 = g[(j+2)*dim+i+1];
            c2 = g[(j+2)*dim+i+2];
            c3 = g[(j+2)*dim+i+3];

            g[(j+2)*dim+i+0] = c0 || g[(i+0)*dim+j+2];
            g[(j+2)*dim+i+1] = c1 || g[(i+1)*dim+j+2];
            g[(j+2)*dim+i+2] = c2 || g[(i+2)*dim+j+2];
            g[(j+2)*dim+i+3] = c3 || g[(i+3)*dim+j+2];

            c0 = g[(j+3)*dim+i+0];
            c1 = g[(j+3)*dim+i+1];
            c2 = g[(j+3)*dim+i+2];
            c3 = g[(j+3)*dim+i+3];

            g[(j+3)*dim+i+0] = c0 || g[(i+0)*dim+j+3];
            g[(j+3)*dim+i+1] = c1 || g[(i+1)*dim+j+3];
            g[(j+3)*dim+i+2] = c2 || g[(i+2)*dim+j+3];
            g[(j+3)*dim+i+3] = c3 || g[(i+3)*dim+j+3];
        }
    }

    long remaining = j;

    for (i=0; i<remaining; i++)
        for (j=remaining; j<dim; j++)
            g[j*dim+i] = g[j*dim+i] || g[i*dim+j];

    for (; i<dim; i++)
        for (j=0; j<dim; j++)
            g[j*dim+i] = g[j*dim+i] || g[i*dim+j];
}

void init_graph(int *g, long dim) {
    long i, j;

    srand(time(0));

    for (i=0; i<dim; i++)
        for (j=0; j<dim; j++)
            g[i*dim+j] = rand() % 7 == 0;

    for (i=0; i<dim; i++)
        g[i*dim+i] = 1;
}




#define SIZE1 10
#define SIZE2 34080

int main() {

    /* 6.45 */
    int m1[SIZE1][SIZE1];
    int m2[SIZE1][SIZE1];

    init_matrix(m1, SIZE1);

    transpose(m2, m1, SIZE1);

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

    init_matrix(m3, SIZE2);

    transpose(m4, m3, SIZE2);

    assert(check_transpose(m3,m4,SIZE2));

    free(m3);
    free(m4);

    /* 6.46 */
    int g[5][5];
    init_graph(g, 5);
    graph_convert(g, 5);

    int *g1 = malloc((long)SIZE2 * (long)SIZE2 * sizeof(int));
    if (g1 == NULL) {
        printf("malloc failed for g1\n");
        return 1;
    }
    init_graph(g1, SIZE2);
    graph_convert(g1, SIZE2);
    free(g1);


    return 0;
}

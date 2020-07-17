#include "stdio.h"
#include "stdlib.h"

#define floatsize sizeof(float)
#define a(x, y) a[x * M + y]

int main(int argc, char** argv)
{
    int M, N;
    int i, j, k;
    float* a;
    FILE* fda;
    fda = fopen("data.txt", "r");
    fscanf(fda, "%d %d", &M, &N);
    if (M != N) {
        puts("The input is error!");
        exit(0);
    }
    a = (float*)malloc(floatsize * M * N);
    for (i = 0; i < M; i++) {
        for (j = 0; j < M; j++) {
            fscanf(fda, "%f", a + i * M + j);
        }
    }
    fclose(fda);

    // for (i = 0; i < M; i++)
    // {
    //     for ( j = 0; j < M; j++)
    //     {
    //         printf("%f\t",a(i,j));
    //     }
    //     printf("\n");

    // }

    for (i = 0; i < M; i++) {
        a(i, i) = 1 / a(i, i);
        for (j = 0; j < M; j++) {
            if (j != i) {
                a(i, j) = a(i, j) * a(i, i);
            }
        }
        for (k = 0; k < M; k++) {
            for (j = 0; j < M; j++) {
                if (k != i && j != i) {
                    a(k, j) = a(k, j) - a(k, i) * a(i, j);
                }
            }
        }
        for (k = 0; k < M; k++) {
            if (k != i) {
                a(k, i) = -a(k, i) * a(i, i);
            }
        }
    }
	printf("done\n");

    // for (i = 0; i < M; i++)
    // {
    //     for ( j = 0; j < M; j++)
    //     {
    //         printf("%f\t",a(i,j));
    //     }
    //     printf("\n");

    // }
    // system("pause");
    return 0;
}

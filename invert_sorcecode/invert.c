#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#define a(x, y) a[x * M + y]
/*A 为 M*M 矩阵*/
#define A(x, y) A[x * M + y]
#define floatsize sizeof(float)
#define intsize sizeof(int)
int M, N;
float* A;
int my_rank;
int p;
MPI_Status status;
void fatal(char* message)
{
    printf("%s\n", message);
    exit(1);
}
void Environment_Finalize(float* a, float* f)
{
    free(a);
    free(f);
}
int main(int argc, char** argv)
{
    int i, j, k, my_rank, group_size;
    int i1, i2;
    int v, w;
    int m;
    float* f;
    float* a;
    FILE* fdA;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,
        &group_size);
    MPI_Comm_rank(MPI_COMM_WORLD,
        &my_rank);
    p = group_size;
    if (my_rank == 0) {
        fdA = fopen("data.txt", "r");
        fscanf(fdA, "%d %d", &M, &N);
        if (M != N) {
            puts("The input is error!");
            exit(0);
        }
        A = (float*)malloc(floatsize * M * M);
        for (i = 0; i < M; i++) {
            for (j = 0; j < M; j++)
                fscanf(fdA, "%f", A + i * M + j);
        }
        fclose(fdA);
        //printf("Input of file \"dataIn.txt\"\n");
        //printf("%d\t%d\n", M, M);
        //for (i = 0; i < M; i++) {
        //    for (j = 0; j < M; j++)
        //        printf("%f\t", A(i, j));
        //    printf("\n");
        //}
    }
    /*0 号处理器将 M 广播给所有处理器*/
    MPI_Bcast(&M, 1, MPI_INT, 0,
        MPI_COMM_WORLD);
    m = M / p;
    if (M % p != 0)
        m++;
    /*各处理器为主行元素建立发送和接收缓冲
区*/
    f = (float*)malloc(sizeof(float) * M);
    /*分配至各处理器的子矩阵大小为 m*M*/
    a = (float*)malloc(sizeof(float) * m * M);
    if (a == NULL || f == NULL)
        fatal("allocate error\n");
    /*0号处理器采用行交叉划分将矩阵 A划分为
m*M 的 p 块子矩阵，依次发送给 1 至 p-1
号处理器*/
    if (my_rank == 0) {
        for (i = 0; i < m; i++)
            for (j = 0; j < M; j++)
                a(i, j) = A(i * p, j);
        for (i = 0; i < M; i++)
            if ((i % p) != 0) {
                i1 = i % p;
                i2 = i / p + 1;
                MPI_Send(&A(i, 0), M,
                    MPI_FLOAT, i1, i2,
                    MPI_COMM_WORLD);
            }
    } else {
        for (i = 0; i < m; i++)
            MPI_Recv(&a(i, 0), M, MPI_FLOAT,
                0, i + 1, MPI_COMM_WORLD,
                &status);
    }
    for (i = 0; i < m; i++)
        for (j = 0; j < p; j++) {
            /*j 号处理器负责广播主行元素*/
            if (my_rank == j) {
                v = i * p + j;
                a(i, v) = 1 / a(i, v);
                for (k = 0; k < M; k++)
                    if (k != v)
                        a(i, k) = a(i, k) * a(i, v);
                for (k = 0; k < M; k++)
                    f[k] = a(i, k);
                MPI_Bcast(f, M, MPI_FLOAT,
                    my_rank,
                    MPI_COMM_WORLD);
            } else {
                v = i * p + j;
                MPI_Bcast(f, M, MPI_FLOAT, j, MPI_COMM_WORLD);
            }
            /*其余处理器则利用主行对其 m行
行向量做行变换*/
            if (my_rank != j) {
                for (k = 0; k < m; k++)
                    for (w = 0; w < M; w++)
                        if (w != v)
                            a(k, w) = a(k, w)
                                - f[w] * a(k, v);
                for (k = 0; k < m; k++)
                    a(k, v) = -f[v] * a(k, v);
            }
            /*发送主行数据的处理器利用主行
对其主行之外的 m-1 行行向
量做行变换*/
            if (my_rank == j) {
                for (k = 0; k < m; k++)
                    if (k != i) {
                        for (w = 0; w < M; w++)
                            if (w != v)
                                a(k, w) = a(k, w) - f[w] * a(k, v);
                    }
                for (k = 0; k < m; k++)
                    if (k != i)
                        a(k, v) = -f[v] * a(k, v);
            }
        }
    /*0号处理器从其余各处理器中接收子矩阵 a，
得到经过变换的逆矩阵 A*/
    if (my_rank == 0) {
        for (i = 0; i < m; i++)
            for (j = 0; j < M; j++)
                A(i * p, j) = a(i, j);
    }
    if (my_rank != 0) {
        for (i = 0; i < m; i++)
            for (j = 0; j < M; j++)
                MPI_Send(&a(i, j), 1,
                    MPI_FLOAT, 0, my_rank,
                    MPI_COMM_WORLD);
    } else {
        for (i = 1; i < p; i++)
            for (j = 0; j < m; j++)
                for (k = 0; k < M; k++) {
                    MPI_Recv(&a(j, k), 1,
                        MPI_FLOAT, i, i,
                        MPI_COMM_WORLD,
                        &status);
                    A((j * p + i), k) = a(j, k);
                }
    }
//    if (my_rank == 0) {
//printf("\nOutput of MatrixA'sinversion\n");
//for(i=0;i<M;i++)
//{
//            for (j = 0; j < M; j++)
//                printf("%f\t", A(i, j));
//            printf("\n");
//}
//    }
    MPI_Finalize();
    Environment_Finalize(a, f);
    return (0);
    free(A);
}

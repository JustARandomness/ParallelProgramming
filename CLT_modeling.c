#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void to_txt(int N, double * x, char * name) {
    // функция для записи результатов в файл
    FILE * fp = fopen(name,"w");
    for (size_t i = 0; i < N; i++)
    {
        fprintf(fp,"%.15e\n",x[i]);
    }
    fclose(fp);
}

int main(int argc, char **argv) {
    int pid, nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    const int N = 5000;
    double buffer[N];
    int K = 100;
    
    // каждый процесс генерирует K наборов случайных чисел
    // и суммирует их в соответствующей ячейке
    for (size_t i = 0; i < N; i++) {
        for (int k = 0; k < K; k++) {
            double x = (1.0*rand())/(1.0*RAND_MAX);
            buffer[i] += x;
        }
    }

    // создадим на корневом процессе приемный буфер, 
    // который будет содержать суммы всех случайных чисел
    double *res;
    if (pid == 0)
        res = calloc(N,sizeof(double));
    MPI_Reduce(buffer,res,N,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if (pid == 0)
        to_txt(N,res,"result.csv");
    
    free(res);
    MPI_Finalize();
    return 0;
}
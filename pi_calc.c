#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>
#include <math.h>

double func(double x){
    return 4.0/(1.0 + x*x);
}

int main(int argc, char *argv[])
{
    int pid, nproc;
    int N, Np, kmin, kmax;
    double h, part_sum, sum_total, time;
    MPI_Status status;
    // параметры разбиения
    N = 2e9;
    h = 1.0/N;
    // MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nproc);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);
    Np = N/nproc; // число интервалов на каждом процессе
    if (pid == 0) time = MPI_Wtime(); // засекаем время
    if (pid < nproc - 1){
        // на всех процессах кроме последнего выставляем диапазон индексов
        kmin = pid*Np; 
        kmax = (pid + 1)*Np;
    }
    else{
        // выставляем диапазон индексов для последнего процесса
        kmin = pid*Np;
        kmax = N - 1;
    }

    part_sum = 0.0;
    for (int k = kmin + 1; k < kmax + 1; k++)
    {
        part_sum += 0.5*(func(k*h) + func((k - 1)*h))*h; // считаем части интеграла
    }
    if (pid != 0){
        // отправляем результаты со всех процессов на корневой
        MPI_Send(&part_sum,1,MPI_DOUBLE,0,123,MPI_COMM_WORLD);
    }
    // на корневом процессе надо принять все результаты и сложить
    if (pid == 0){
        double temp;
        sum_total = part_sum;
        for (size_t i = 1; i <= nproc - 1; i++)
        {
            // должно быть nproc-1 операций приема
            // поскольку не знаем, откуда придет и в каком порядке, ставим MPI_ANY_SOURCE
            MPI_Recv(&temp,1,MPI_DOUBLE,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            sum_total += temp;
        }
        time = MPI_Wtime() - time;
        printf("total sum = %e, error = %e, time = %f", sum_total, sum_total-M_PI, time);
    }

    MPI_Finalize();
    return 0;
}
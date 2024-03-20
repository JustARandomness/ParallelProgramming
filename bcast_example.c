#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int pid,nprocs;
    int N = 100000;
    int sum = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);
    int *buf = calloc(N,sizeof(int));
    if (pid == 0){
        for (int i=0;i<N; i++){
            buf[i]=i*i;
        }

    }
    MPI_Barrier(MPI_COMM_WORLD);
    double t = MPI_Wtime();
    MPI_Bcast(buf,N,MPI_INT,0,MPI_COMM_WORLD);

    for (size_t i=0;i<N;i++){
        sum += buf[i];
    }
    printf("pid = %d sum = %d \n",pid,sum);
    
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Time for %d processes is %f", nprocs, MPI_Wtime() - t);
    free(buf);
    MPI_Finalize();
    return 0;

}
#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int pid, nproc;
    int N = 10;
    int sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    int *buf = calloc(N,sizeof(int));
    if (pid == 0) {
        for (int i = 0; i < N; i++) {
            buf[i] = i*i;
        }
    }

    MPI_Bcast(buf, N, MPI_INT, 
                    0, MPI_COMM_WORLD);

    for (size_t i = 0; i < N; i++)
    {
        sum += buf[i];
    }
    printf("pid = %d sum = %d \n",pid, sum);
    
    free(buf);
    MPI_Finalize();
    return 0;
}
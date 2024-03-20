#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pid, nprocs;
    int buffer = 0;
    int buffer_count = 1;
    double t;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);
    MPI_Barrier(MPI_COMM_WORLD);
    if (pid == 0) {
        t = MPI_Wtime();
        sleep(3);
        MPI_Send(&buffer, buffer_count, MPI_INT, 1, 0, MPI_COMM_WORLD);
        sleep(6);
    }
    if (pid == 1) {
        sleep(6);
        MPI_Recv(&buffer, buffer_count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        sleep(3);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (pid == 0) {
        printf("%f",MPI_Wtime() - t);
    }
    MPI_Finalize();
    return 0;
}
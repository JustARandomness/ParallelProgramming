#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int pid, nproc, left, right;
    int buffer, buffer2;
    MPI_Status status;
 
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    buffer = pid;
 
    right = (pid + 1) % nproc;
    left = pid - 1;
    if (left < 0)
        left = nproc - 1;
 
    MPI_Sendrecv( &buffer, 1, MPI_INT, left, 123, 
                  &buffer2, 1, MPI_INT, right, 123, 
                  MPI_COMM_WORLD, &status);
    printf("proc %d sent %d and received %d", pid, buffer, buffer2);
 
    MPI_Finalize();
    return 0;
}
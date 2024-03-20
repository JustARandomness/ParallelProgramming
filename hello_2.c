#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, nprocs;
    double a = 10;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    a += rank;
    printf("Parallel: a = %f \n", a);
    MPI_Finalize();

    printf("After: a = %f \n",a);
    return 0;
}
#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, nprocs;
    double a = 10;
    printf("%f \n", a);
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf("%d gives %f \n", rank, a);
    MPI_Finalize();
    printf("MPI Finalized \n");
    printf("%d gives %f \n", rank, a);
    return 0;
}
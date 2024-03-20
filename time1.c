#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, nprocs, i;
    double a,t,dt;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    dt = MPI_Wtick();
    t = MPI_Wtime();
    for (i = 0; i<100000; i++){
        a = 0;
    }
    t = MPI_Wtime() - t;
    printf("t = %f, dt = %f from %d \n",t,dt,rank);
    MPI_Finalize();
    return 0;
}

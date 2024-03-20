#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    int pid, nprocs, nrecv;
    double x;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);
    printf("Hello from %d of %d \n", pid, nprocs);

    nrecv = 1; // указываем адрес
    if (pid == 0){
        // операции _только_ для pid = 0
        x = 10.0;
        MPI_Send( &x, 1, MPI_DOUBLE, nrecv, 
                    123, MPI_COMM_WORLD);
        printf("%d sending x = %f to %d",pid,x,nrecv);
    }
    if (pid == nrecv){
        // операции _только_ для pid = nrecv
        MPI_Recv( &x, 1, MPI_DOUBLE, 0, 
                    123, MPI_COMM_WORLD, &status);
        printf("%d received x = %f",nrecv,x);
    }
    MPI_Finalize();
    return 0;
}
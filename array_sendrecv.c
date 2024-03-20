#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

void print_vector(int N, int *u)
{
    for (size_t i = 0; i < N; i++)
    {
        if (i == 0){
        printf("[ ");
        }
        if (i != N-1){
        printf("%d, ",u[i]);
        }
        else{
        printf("%d ]\n",u[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    int pid, nprocs, nrecv;
    int len = 3;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&pid);

    nrecv = 1; // указываем адрес
    if (pid == 0){
        // операции _только_ для pid = 0
        int *x = calloc(len,sizeof(int));
        for (int i = 0; i < len; i++){
            x[i] = i*i;
        }
        printf("proc %d have x = ",pid);
        print_vector(len,x);
        MPI_Send( x, len, MPI_INT, nrecv, 
                    123, MPI_COMM_WORLD);
        free(x);
    }
    if (pid == nrecv){
        // операции _только_ для pid = nrecv
        int *x = calloc(len,sizeof(int));
        int count;
        MPI_Recv( x, len, MPI_INT, MPI_ANY_SOURCE, 
                    MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        printf("proc %d received x = ",pid);
        printf("proc %d received from %d with tag %d \n", pid, status.MPI_SOURCE, status.MPI_TAG);
        printf("Number of elements is %d \n", count);
        print_vector(len,x);
        free(x);
    }
    MPI_Finalize();
    return 0;
}
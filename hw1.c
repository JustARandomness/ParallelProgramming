#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int pid, nproc, nrecv;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    int len_sent = 4;
    int len_rcvd = 4;

    if (pid == 0)
    {
        char *x_sent = malloc(len_sent);
        for (int i = 0; i < len_sent - 1; ++i)
        {
            x_sent[i] = 'a';
        }
        x_sent[3] = '\0';
        char *x_rcvd = malloc(len_rcvd);
        MPI_Send(x_sent, len_sent, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(x_rcvd, len_rcvd, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        printf("process %d sent %s\n", pid, x_sent);
        printf("process %d recieved %s\n", pid, x_rcvd);
        free(x_sent);
        free(x_rcvd);
    }
    else
    {
        char *x_sent = malloc(len_sent);
        char *x_rcvd = malloc(len_rcvd);
        for (int i = 0; i < len_sent - 1; ++i)
        {
            x_sent[i] = 'b';
        }
        x_sent[3] = '\0';
        MPI_Recv(x_rcvd, len_rcvd, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Send(x_sent, len_sent, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        printf("process %d sent %s\n", pid, x_sent);
        printf("process %d recieved %s\n", pid, x_rcvd);
        free(x_sent);
        free(x_rcvd);
    }

    MPI_Finalize();
    return 0;
}
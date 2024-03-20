
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int pid, nproc;
    int MAX_STRING = 100;
    char greeting[MAX_STRING];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    if (pid != 0)
    {
        sprintf(greeting, "Greetings from process %d of %d!", pid, nproc);
        MPI_Sendrecv_replace(greeting, MAX_STRING, MPI_CHAR, 0, 0, 0, 0, MPI_COMM_WORLD, &status);
    }
    else
    {
        printf("Greetings from process %d of %d!\n", pid, nproc);
        for (int source = 1; source < nproc; ++source)
        {
            MPI_Sendrecv_replace(greeting, 100, MPI_CHAR, source, 0, source, 0, MPI_COMM_WORLD, &status);
            printf("%s\n", greeting);
        }
    }

    MPI_Finalize();
    return 0;
}
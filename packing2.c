#define MSMPI_NO_SAL
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

#define MASTER 0
#define STRLEN 25

int main(int argc, char *argv[])
{
    int rank, size;
    int i;
    char c[7] = "My name";
    char buffer[15];
    int position = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size != 2)
    {
        printf("Только для двух процессов.\n");
        fflush(stdout);
        MPI_Finalize();
        return 1;
    }

    if (rank == 0)
    {
        for (int i = 0; i < 7; i++)
        {
            if (i%2 == 0)
                MPI_Pack(&c[i], 1, MPI_CHAR, buffer, 15, &position, MPI_COMM_WORLD);
        }
        MPI_Send(buffer, position, MPI_PACKED, 1, 100, MPI_COMM_WORLD);
    }

    if (rank == 1)
    {
        MPI_Recv(buffer, 15, MPI_PACKED, 0, 100, MPI_COMM_WORLD, &status);
        MPI_Unpack(buffer, 15, &position, c, 100, MPI_CHAR, MPI_COMM_WORLD);
        printf("%s\n",c);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}
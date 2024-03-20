#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int pid, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    srand(time(NULL) + pid);

    int N = 4;
    int num_per_proc = N / nprocs;
    MPI_Barrier(MPI_COMM_WORLD);
    int s = 0;
    int *points_array = calloc(num_per_proc, sizeof(int));
    for (int i = 0; i < num_per_proc; ++i)
    {
        points_array[i] = rand() % 201 - 100;
        printf("%d\n", points_array[i]);
        s += points_array[i];
    }

    int recv_buf = 0;
    MPI_Allreduce(&s, &recv_buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    float avg_coord = (float)recv_buf / (nprocs * num_per_proc);
    
    if (pid == 0)
    {
        printf("Coordinates are %f\n", avg_coord);
    }

    for (int i = 0; i < num_per_proc; ++i) {
        printf("Process id is %d\nDistance between point %d and center of mass is %f\n", pid, points_array[i], fabs(points_array[i] - recv_buf));
    }

    MPI_Finalize();
    return 0;
}

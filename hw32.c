#include "/opt/homebrew/opt/openmpi/include/mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int pid, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    int num_even_procs = 0;

    if (nprocs % 2 != 0) {
        num_even_procs = (int)(nprocs / 2) + 1;
    }
    else {
        num_even_procs = (int)(nprocs / 2);
    }

    int *prime_ranks = calloc(num_even_procs, sizeof(int));

    for (int i = 0; i < num_even_procs; ++i) {
        prime_ranks[i] = i * 2;
    }

    MPI_Group even_group;
    MPI_Group odd_group;
    MPI_Group_incl(world_group, (int)(nprocs / 2 - nprocs % 2), prime_ranks,
                   &even_group);
    MPI_Group_excl(world_group, (int)(nprocs / 2 - nprocs % 2), prime_ranks,
                   &odd_group);

    MPI_Group odd_group_diff;
    MPI_Group_difference(world_group, even_group, &odd_group_diff);


    int rank;
    MPI_Group_rank(odd_group, &rank);
    if (rank == MPI_UNDEFINED) {
        printf("rank: %d, Isn't part of this group\n", pid);
    }
    else {
        printf("rank: %d, rank in group: %d\n", pid, rank);
    }

    int a = 2;
    MPI_Comm even_comm;
    MPI_Comm_create(MPI_COMM_WORLD, even_group, &even_comm);
    if (even_comm != MPI_COMM_NULL) {
        MPI_Bcast(&a, 1, MPI_INT, 2, even_comm);
    }
    if ((pid % 2 != 0) && (pid != 1)) {
        printf("%d", a);
    }

    free((int *)prime_ranks);
    MPI_Finalize();
    return 0;
}

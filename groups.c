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

  const int *prime_ranks = calloc((int)(nprocs / 2 - nprocs % 2), sizeof(int));

  MPI_Group even_group;
  MPI_Group odd_group;
  MPI_Group_incl(world_group, (int)(nprocs / 2 - nprocs % 2), prime_ranks,
                 &even_group);
  MPI_Group_excl(world_group, (int)(nprocs / 2 - nprocs % 2), prime_ranks,
                 &odd_group);

  MPI_Group odd_group_diff;
  int temp = MPI_Group_difference(world_group, even_group, &odd_group_diff);

  int result = 0;
  MPI_Group_compare(odd_group_diff, odd_group, &result);
  MPI_Barrier(MPI_COMM_WORLD);
  if (pid == 0) {
    printf("%b", result == MPI_IDENT);
  }

  free((int *)prime_ranks);
  MPI_Finalize();
  return 0;
}

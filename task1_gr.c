#include <stdio.h>
#include "mpi.h"


int main(int argc, char** argv){
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Group group_world;
    MPI_Comm_group(MPI_COMM_WORLD, &group_world);
    
    const int n = 4;
    const int ranks[4] = {0, 2, 4, 6};


    MPI_Group group1_incl, group2_excl, group2_difference;
    MPI_Group_incl(group_world, n, ranks, &group1_incl);
    MPI_Group_excl(group_world, n, ranks, &group2_excl);
    
    MPI_Group_difference(group_world, group1_incl, &group2_difference);
    
    int result;
    MPI_Group_compare(group2_excl, group2_difference, &result);

    if (result == MPI_IDENT) {
        printf("The same group\n"); // члены групп и их номера совпадают
    }
    MPI_Finalize();
    return 0;
}

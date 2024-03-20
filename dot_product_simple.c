#define MSMPI_NO_SAL
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
    int pid, nproc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    double ak = pid*pid;
    double bk = log10(pid + 1.0);

    double prod = ak * bk;

    double result;
    MPI_Reduce(&prod, &result, 1, MPI_DOUBLE,
                    MPI_SUM, 0, MPI_COMM_WORLD);

    if (pid == 0){
        printf("Reduced value = %.15f \n",result);

        double valid = 0;
        for (int i = 0; i < nproc; i++)
            valid += (i*i) * log10(i + 1.0);
        printf("Valid value = %.15f \n",valid);
    }
    MPI_Finalize();
    return 0;
}
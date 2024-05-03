#include <stdio.h>
#include "/opt/homebrew/opt/libomp/include/omp.h"
int main(int argc, char *argv[])
{
    double start_time, end_time, tick;
    start_time = omp_get_wtime();
    end_time = omp_get_wtime();
    tick = omp_get_wtick();
    printf("Time: %f\n", end_time-start_time);
    printf("Timer accuracy: %f\n", tick);
}

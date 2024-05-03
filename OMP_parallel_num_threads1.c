#include <stdio.h>
#include <omp.h>

int main()
{
    omp_set_num_threads(4);
    int n;
    printf("Conseq section 1\n");
    n = omp_get_num_threads();
    printf("num threads = %d \n");

    #pragma omp parallel
    {
        printf("Parallel section 1\n");
        n = omp_get_num_threads();
        printf("num threads = %d \n");
    }
    printf("Conseq section 2\n");
    return 0;
} 
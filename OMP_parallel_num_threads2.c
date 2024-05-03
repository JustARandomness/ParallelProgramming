#include <stdio.h>
#include <omp.h>

int main()
{
    int n;
    printf("Conseq section 1\n");
    n = omp_get_num_threads();
    printf("num threads = %d \n");

    #pragma omp parallel num_threads(4)
    {
        printf("Parallel section 1\n");
        n = omp_get_num_threads();
        printf("num threads = %d \n");
    }
    printf("Conseq section 2\n");
    return 0;
} 
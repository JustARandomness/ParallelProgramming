#include <stdio.h>
#include <omp.h>

int main(void)
{
    int n = 3;
    #pragma omp parallel if(n > 4)
    {
    printf("Hello, world!\n");
    }
    return 0;
}
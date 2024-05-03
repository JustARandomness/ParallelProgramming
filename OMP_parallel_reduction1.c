#include <stdio.h>
#include <omp.h>

int main()
{
    int count = 0;
    #pragma omp parallel reduction (+: count)
    // значение count копируется в каждом потоке из значения на мастер-потоке
    // +: count означает, что по завершении параллельной секции значения count с каждого потока будут прибавлены к значению этой переменной на мастере
    {
        count++;
        printf("count: %d\n", count);
    }
    printf("reduction result: %d\n", count);
} 
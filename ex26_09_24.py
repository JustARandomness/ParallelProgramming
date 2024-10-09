from mpi4py import MPI
import numpy as np

def f(x):
    return 4 / (1 + np.pow(x, 2))


start = MPI.Wtime()
comm = MPI.COMM_WORLD
my_rank = comm.Get_rank()
nprocs = comm.Get_size()

a = 0
b = 1

N = 100000000

num_per_proc = N // nprocs

len_per_proc = (b - a) / nprocs

my_a = len_per_proc * my_rank
my_b = len_per_proc * (my_rank + 1)

h = (b - a) / (N - 1)

x = np.linspace(my_a, my_b, num_per_proc, endpoint=False)

s = h * np.sum((f(x[:-2]) + 4 * f(x[1:-1]) + f(x[2:]))) / 6

final_sum = comm.reduce(s, MPI.SUM)
end = MPI.Wtime()
if my_rank == 0:
    print(f'Integral equals: {final_sum}, calculated for {end - start} ms')
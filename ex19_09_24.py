from mpi4py import MPI
import numpy as np

def f(x):
    return 4 / (1 + np.pow(x, 2))

world_comm = MPI.COMM_WORLD
nprocs = world_comm.Get_size()
rank = world_comm.Get_rank()

N = 10000000

num_per_proc = N // nprocs

a = 0
b = 1

len_per_proc = (b - a) / nprocs

my_a = len_per_proc * rank
my_b = len_per_proc * (rank + 1)

start = MPI.Wtime()
my_x = np.linspace(my_a, my_b, num_per_proc, endpoint=False)

s = np.sum(((f(my_x[:-1]) + f(my_x[1:])) * (my_x[1:] - my_x[:-1])) / 2)

final_sum = world_comm.reduce(s, MPI.SUM)
time = MPI.Wtime() - start

if rank == 0:
    print(f'Integral equals: {final_sum}, time: {time}')

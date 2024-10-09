from mpi4py import MPI
import numpy as np

world_comm = MPI.COMM_WORLD
my_rank = world_comm.Get_rank()
nprocs = world_comm.Get_size()

if my_rank == 0:
    data = np.ones((8, 10))
else:
    data = None

data = world_comm.scatter(data)
world_comm.Barrier()

print(f'process {my_rank}, data: {data}')


from mpi4py import MPI


world_comm = MPI.COMM_WORLD
my_rank = world_comm.Get_rank()
nprocs = world_comm.Get_size()

if my_rank == nprocs - 1:
    temp = 10000

if nprocs == 1:
    print('В программе только 1 процесс, пересылка закончена')
else:
    if my_rank == nprocs - 1:
        world_comm.send(temp,  nprocs - 2)
        print(f'С процесса {my_rank} отправлено число {temp}')
    else:
        if my_rank != 0:
            temp = world_comm.recv(source=my_rank + 1)
            if my_rank % 2 == 0:
                print(f'На процессе {my_rank} получено число {temp}, на процесс {my_rank - 1} отправлено число {-temp}')
                world_comm.send(-temp, dest=my_rank - 1)
            else:
                print(f'На процессе {my_rank} получено число {temp}, на процесс {my_rank - 1} отправлено число {temp}')
                world_comm.send(temp, dest=my_rank - 1)
        else:
            temp = world_comm.recv(source=my_rank + 1)
            print(f'На процессе 0 получено число {temp}, пересылка завершена')

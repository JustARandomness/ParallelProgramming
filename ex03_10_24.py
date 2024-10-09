import math
from random import randint, uniform
from mpi4py import MPI as mpi
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.lines as mlines
import matplotlib.path as mpath

comm = mpi.COMM_WORLD
rank = comm.Get_rank()
nprocs = comm.Get_size()


r = 0.03
frames = 180
if rank == 0:
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    plt.grid(True)
    ax = plt.gca()
    ax.set_aspect("equal")
    
V = 0.01
alpha = math.radians(randint(0, 359))
Vx = V * math.cos(alpha)
Vy = V * math.sin(alpha)
x, y = uniform(r, 1 - r), uniform(r, 1 - r)
X = [x]
Y = [y]
for i in range(frames):
    x += Vx
    y += Vy
    if x < r or x > 1 - r:
        Vx = -Vx
    if y < r or y > 1 - r:
        Vy = -Vy
    X.append(x)
    Y.append(y)

comm.Barrier()

if rank > 0:
    comm.send(X, dest=0)
    comm.send(Y, dest=0)
if rank == 0:
    X_all, Y_all = [], []
    X_all.append(X)
    Y_all.append(Y)
    colors = ['r', 'g', 'b', 'c', 'm', 'y']
    for i in range(1, nprocs):
        X_all.append(comm.recv(source=i))
        Y_all.append(comm.recv(source=i))
    for i in range(nprocs):
        circle = mpatches.Circle((X_all[i][0], Y_all[i][0]), radius=r, fill=True, color=colors[i % len(colors)])
        ax.add_patch(circle)
    file = './Animation/Circle' + str(0) + '.png'
    for t in range(1, frames):
        for i in range(nprocs):
            circle = mpatches.Circle((X_all[i][t - 1], Y_all[i][t - 1]), radius=r, fill=True, color='w')
            ax.add_patch(circle)
            circle = mpatches.Circle((X_all[i][t], Y_all[i][t]), radius=r, fill=True, color=colors[i % len(colors)])
            ax.add_patch(circle)
        file = './Animation/Circle' + str(t) + '.png'
        plt.savefig(file, dpi=200)
            
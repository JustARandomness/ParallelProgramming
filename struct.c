#define MSMPI_NO_SAL
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#define NELEM 25

typedef struct Particle{
  float x, y, z;
  float velocity;
  int  n, type;
  } Particle;

int main(int argc, char *argv[])  {
  int nproc, pid, source=0, dest, tag=1, i;

  Particle p[NELEM], particles[NELEM];
  MPI_Datatype particletype, oldtypes[2];
  int blockcounts[2];

  MPI_Aint displacements[2], lb, extent;

  MPI_Status stat;
  MPI_Request req;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  // описание 4х элементов типа float - координаты и скорость
  displacements[0] = 0;
  oldtypes[0] = MPI_FLOAT;
  blockcounts[0] = 4;

  // описание 2х элемнтов типа int - номер частицы и ее тип (1 или 0)
  // сначала надо записать протяженность типа MPI_FLOAT
  MPI_Type_get_extent(MPI_FLOAT, &lb, &extent);
  displacements[1] = 4 * extent;
  oldtypes[1] = MPI_INT;
  blockcounts[1] = 2;

  // define structured type and commit it
  MPI_Type_create_struct(2, blockcounts, displacements, oldtypes, &particletype);
  MPI_Type_commit(&particletype);

  // начнем асинхронный прием для отправки самому себе
  MPI_Irecv(p, NELEM, particletype, source, tag, MPI_COMM_WORLD, &req);
  // корневой процесс создает частицы
  if (pid == 0) {
    for (i=0; i<NELEM; i++) {
      particles[i].x = i * 1.0;
      particles[i].y = i * -1.0;
      particles[i].z = i * 1.0; 
      particles[i].velocity = 0.25;
      particles[i].n = i;
      particles[i].type = i % 2; 
    }
    // отправка с корневого процесса себе самому и остальным
    for (i=0; i<nproc; i++) 
      MPI_Send(particles, NELEM, particletype, i, tag, MPI_COMM_WORLD);
  }

  // ждем получения
  MPI_Wait(&req, MPI_STATUS_IGNORE);

  printf("pid= %d   %3.2f %3.2f %3.2f %3.2f %d %d\n", pid,p[pid].x,
      p[pid].y,p[pid].z,p[pid].velocity,p[pid].n,p[pid].type);

  MPI_Type_free(&particletype);
  MPI_Finalize();
  return 0;
}
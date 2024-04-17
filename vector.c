#include "/opt/homebrew/opt/open-mpi/include/mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE 4

int main(int argc, char *argv[]) {
  int nproc, pid, source = 0, dest, tag = 1, i;
  float a[SIZE][SIZE] = {1.0, 2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,
                         9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0};
  float b[SIZE];

  MPI_Status stat;
  MPI_Request req;
  MPI_Datatype columntype;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  MPI_Type_vector(SIZE /*число элементов в столбце*/,
                  1 /*количество элементов в строке - длина блока*/,
                  SIZE /*каждые 4 элемента*/, MPI_FLOAT, &columntype);
  MPI_Type_commit(&columntype);

  /*рассылка столбцов*/
  // MSMPI не поддерживает блокирующие отправку и прием самому себе,
  // поэтому используем асинхронный прием
  if (nproc == SIZE) {
    MPI_Irecv(b, SIZE, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &req);
    if (pid == 0) {
      MPI_Send(&a[0], SIZE, columntype, i, 1, MPI_COMM_WORLD);
    }
    MPI_Wait(&req, MPI_STATUS_IGNORE);
    printf("pid= %d  b= %3.1f %3.1f %3.1f %3.1f\n", pid, b[0], b[1], b[2],
           b[3]);
  } else
    printf("Возможно только %d процесса.\n", SIZE);

  MPI_Type_free(&columntype);
  MPI_Finalize();
}

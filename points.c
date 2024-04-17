#define MSMPI_NO_SAL
#include <stdio.h>
#include <mpi.h>

typedef struct Point {
  double x, y, z;
} Point;

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Datatype dt_point;
  MPI_Type_contiguous(3, MPI_DOUBLE, &dt_point);
  MPI_Type_commit(&dt_point);
  
  const int n_points = 10;
  Point data[n_points];

  if (rank == 0) {
    for (int i=0; i < n_points; ++i) {
      data[i].x = 1.0*i;
      data[i].y = -1.0*i;
      data[i].z = 1.0*i*i;
    }
    MPI_Send(data, n_points, dt_point, 1, 0, MPI_COMM_WORLD);
  }
  else { // Process 1 receives
    MPI_Recv(data, n_points, dt_point, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i=0; i < n_points; ++i) {
        printf("Point # %d: (%f, %f, %f) \n", i, data[i].x, data[i].y, data[i].z);
    }
  }
  MPI_Finalize();
}
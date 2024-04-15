#include "/opt/homebrew/opt/openmpi/include/mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float calculate_module(float x1, float y1, float x2, float y2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int main(int argc, char *argv[]) {
  int pid, nprocs;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  srand(time(NULL) + pid);

  int N = 100;
  int num_per_proc = N / nprocs;
  MPI_Barrier(MPI_COMM_WORLD);
  int x_s = 0;
  int y_s = 0;
  int *x_points_array = calloc(num_per_proc, sizeof(int));
  int *y_points_array = calloc(num_per_proc, sizeof(int));
  for (int i = 0; i < num_per_proc; ++i) {
    x_points_array[i] = rand() % 201 - 100;
    y_points_array[i] = rand() % 201 - 100;
    printf("x: %d, y: %d\n", x_points_array[i], y_points_array[i]);
    x_s += x_points_array[i];
    y_s += y_points_array[i];
  }

  int x_recv_buf = 0;
  int y_recv_buf = 0;
  MPI_Allreduce(&x_s, &x_recv_buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&y_s, &y_recv_buf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  float avg_coord_x = (float)x_recv_buf / (nprocs * num_per_proc);
  float avg_coord_y = (float)y_recv_buf / (nprocs * num_per_proc);

  if (pid == 0) {
    printf("Coordinates are (%f, %f)\n", avg_coord_x, avg_coord_y);
  }

  for (int i = 0; i < num_per_proc; ++i) {
    printf("Process id is %d\nDistance between point (%d, %d) and center of "
           "mass is "
           "%f\n",
           pid, x_points_array[i], y_points_array[i],
           calculate_module(x_points_array[i], y_points_array[i], x_recv_buf,
                            y_recv_buf));
  }

  MPI_Finalize();
  return 0;
}

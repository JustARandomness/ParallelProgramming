#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void print_vector(int N, double *u) {
  for (size_t i = 0; i < N; i++) {
    if (i == 0) {
      printf("[ ");
    }
    if (i != N - 1) {
      printf("%.10f, ", u[i]);
    } else {
      printf("%.10f ]\n", u[i]);
    }
  }
}
void matrix_to_txt(int N, double A[], char *name) {
  FILE *fp;
  char header[] = "output.txt";
  if (name != NULL)
    fp = fopen(name, "w");
  else
    fp = fopen(header, "w");

  for (int i = 0; i < N * N; i++) {
    fprintf(fp, "%.2f", A[i]);
    if ((i + 1) % N == 0)
      fprintf(fp, "\n");
    else
      fprintf(fp, ",");
  }
  fclose(fp);
}

const int MAXIT = 50;
const int points = 2048;

double modulus(double x, double y) { return sqrt(x * x + y * y); }
double iterator(double x, double y) {
  int iter = 0;
  int color;
  double xn = 0, yn = 0;
  double xnew, ynew;
  double radius;
  while (iter < MAXIT) {
    xnew = xn * xn - yn * yn + x;
    ynew = 2.0 * xn * yn + y;
    xn = xnew;
    yn = ynew;
    radius = modulus(xn, yn);
    if (radius > 2.0)
      break;
    iter++;
  }
  if (iter == MAXIT)
    color = -1;
  else
    color = iter;
  return 1.0 * color;
}

int main(int argc, char **argv) {
  double xmin, xmax, ymin, ymax, dx, dy;
  int pid, nproc;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  double *xpoints = NULL, *ypoints = NULL, *ylocal = NULL;
  double *colors = NULL, *colors_local = NULL;

  xmin = -2.0;
  xmax = 1.0;
  dx = xmax - xmin;
  ymin = -1.5;
  ymax = 1.5;
  dy = ymax - ymin;

  xpoints = calloc(points, sizeof(double));
  for (size_t i = 0; i < points; i++) {
    xpoints[i] = xmin + dx * i / points;
  }
  if (pid == 0) {
    ypoints = calloc(points, sizeof(double));
    for (size_t i = 0; i < points; i++) {
      ypoints[i] = ymin + dy * i / points;
    }
    colors =
        calloc(points * points, sizeof(double)); // массив с цветами для сборки
  }

  int Np = points / nproc;
  printf("%d", Np);
  ylocal = calloc(
      Np,
      sizeof(double)); // здесь будут храниться значения y для каждого процесса
  colors_local =
      calloc(points * Np, sizeof(double)); // локальные массивы для вычислений

  MPI_Scatter(ypoints, Np, MPI_DOUBLE, ylocal, Np, MPI_DOUBLE, 0,
              MPI_COMM_WORLD);

  for (size_t i = 0; i < Np; i++) {
    for (size_t j = 0; j < points; j++) {
      colors_local[points * i + j] = iterator(xpoints[j], ylocal[i]);
    }
  }

  MPI_Gather(colors_local, points * Np, MPI_DOUBLE, colors, points * Np,
             MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (pid == 0) {
    matrix_to_txt(points, colors, NULL);
  }

  free(ylocal);
  free(colors_local);
  free(colors);
  free(xpoints);
  free(ypoints);

  MPI_Finalize();
  return 0;
}

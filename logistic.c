#include "/opt/homebrew/opt/openmpi/include/mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int iterations = 500;

void matrix_to_csv_int(int size, double* x, int* y, char* name) {
    FILE* file = fopen(name, "w");
    if (file == NULL) {
        printf("Ошибка чтения файла\n");
        return;
    }
    for (int i = 0; i < size; ++i) {
        fprintf(file, "%f,%d\n", x[i], y[i]);
    }
    fclose(file);
}

void matrix_to_csv_double(int size, double* x, double* y, char* name) {
    FILE* file = fopen(name, "w");
    if (file == NULL) {
        printf("Ошибка чтения файла\n");
        return;
    }
    for (int i = 0; i < size; ++i) {
        fprintf(file, "%f,%f\n", x[i], y[i]);
    }
    fclose(file);
}

int get_num_mods(double x, double r) {
    for (int i = 0; i < iterations - 50; ++i) {
        x = r * x * (1 - x);
    }
    double* x_array = calloc(50, sizeof(double));
    x_array[0] = x;
    int x_arr_size = 50;
    int uniq_cnt = 0;
    for (int i = 1; i < x_arr_size; ++i) {
        x_array[i] = r * x_array[i - 1] * (1 - x_array[i - 1]);
    }
    for (int i = 0; i < x_arr_size; ++i) {
        int flag = 1;
        for (int j = 0; j < x_arr_size; ++j) {
            if (i != j && x_array[i] == x_array[j]) {
                flag = 0;
            }
        }
        if (flag == 1) {uniq_cnt++;};
    }
    return uniq_cnt;
}


int main(int argc, char **argv) {
    int pid, nprocs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int r_arr_size = 200;
    int r_points_per_proc = r_arr_size / nprocs;

    double x_min = 0, x_max = 1, x_step = 0.002;
    double r_min = 0, r_max = 4, r_step = (r_max - r_min) / r_arr_size;

    double *r_arr = NULL, *r_arr_local = NULL;

    int *mods_arr = NULL, *mods_arr_local = NULL;


    if (pid == 0) {
        r_arr = calloc(r_arr_size, sizeof(double));
        mods_arr = calloc(r_arr_size, sizeof(double));
        for (int i = 0; i < r_arr_size; ++i) {
            r_arr[i] = r_min + i * r_step;
        }
    }

    double* x_r_arr = calloc(4, sizeof(double));
    x_r_arr[0] = 2.5;
    x_r_arr[1] = 3.2;
    x_r_arr[2] = 3.5;
    x_r_arr[3] = 3.8;

    r_arr_local = calloc(r_points_per_proc, sizeof(double));
    mods_arr_local = calloc(r_points_per_proc, sizeof(double));

    MPI_Scatter(r_arr, r_points_per_proc, MPI_DOUBLE, 
                r_arr_local, r_points_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double x0 = 0.5;

    for (int i = 0; i < r_points_per_proc; ++i) {
        mods_arr_local[i] = get_num_mods(x0, r_arr_local[i]);
    }

    MPI_Gather(mods_arr_local, r_points_per_proc, MPI_INT, mods_arr, 
                r_points_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    if (pid == 0) {
        matrix_to_csv_int(r_arr_size, r_arr, mods_arr, "mod_r.csv");
    }

    if (pid == 0) {
        double* x_iteration_points = calloc(iterations, sizeof(double));
        double* y_iteration_points = calloc(iterations, sizeof(double));
        x0 = 0.1;
        for (int i = 0; i < 4; ++i) {
            y_iteration_points[0] = x_r_arr[i] * x0 * (1 - x0);
            x_iteration_points[0] = x0;
            for (int j = 1; j < iterations; ++j) {
                y_iteration_points[j] = x_r_arr[i] * y_iteration_points[j - 1] * (1 - y_iteration_points[j - 1]);
                x_iteration_points[j] = x_r_arr[i] * x_iteration_points[j - 1] * (1 - x_iteration_points[j - 1]);
            }
            if (i == 0) {
                matrix_to_csv_double(iterations, x_iteration_points, y_iteration_points, "x_r25.csv");
            }
            else if (i == 1) {
                matrix_to_csv_double(iterations, x_iteration_points, y_iteration_points, "x_r32.csv");
            }
            else if (i == 2) {
                matrix_to_csv_double(iterations, x_iteration_points, y_iteration_points, "x_r35.csv");
            }
            else if (i == 3) {
                matrix_to_csv_double(iterations, x_iteration_points, y_iteration_points, "x_r38.csv");
            }
        }
        free(x_iteration_points);
        free(y_iteration_points);
    }

    free(r_arr);
    free(r_arr_local);
    free(mods_arr);
    free(mods_arr_local);

    MPI_Finalize();
    return 0;
}

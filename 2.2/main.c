#include "matrixlib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*Returns a random number in the range 0 - <range_max>*/
int rand_from_range(int range_max) {
    int result = rand() / (RAND_MAX / range_max + 1);
    return result;
}

/*Return the amount of time elapsed between two timespec instances*/
double time_elapsed(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main(int argc, char *argv[]) {
    // Checking for correct number of arguments
    if (argc != 5) {
        fprintf(stderr, "Program must be called as -> ./main dimension zero_percentage reps threads\n");
        fprintf(stderr, "Can also be called as -> make run D=dimension Z=zero_percentage R=reps T=threads\n");
        fprintf(stderr, "Or as(using default values) -> make run\n");
        return 1;
    }
    // Receiving program inputs
    int dimension = atoi(argv[1]);       // Matrix dimension
    int zero_percentage = atoi(argv[2]); // Percentage of matrix elements with a value of 0
    int reps = atoi(argv[3]);            // Numbers of times multiplication is repeated
    int threads = atoi(argv[4]);         // Number of threads used for parallel execution

    // Timespec initialization
    struct timespec serial_CSRrep_start, serial_CSRrep_finish;
    struct timespec serial_mult_start, serial_mult_finish;
    struct timespec serial_CSRmult_start, serial_CSRmult_finish;

    // HACK:Printing arguments for debugging purposes
    printf("Array dimension is: %d ", dimension);
    printf("Zero_percentage is: %d ", zero_percentage);
    printf("Reps is: %d ", reps);
    printf("Threads is: %d\n", threads);
    // HACK:

    // Useful values
    int total_values = dimension * dimension;
    int zeroes = ceil(total_values * zero_percentage / 100.0);
    printf("There are %d zeroes out of %d values.\n", zeroes, total_values);

    // Generate matrix for multiplication with values from 0-100
    int **mat = (int **)malloc(dimension * sizeof(int *));
    for (int x = 0; x < dimension; x++) {
        mat[x] = (int *)malloc(dimension * sizeof(int));
    }

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            // Setting all values of the matrix to random integers, excluding 0
            int val = rand_from_range(100);
            while (val == 0) {
                val = rand_from_range(100);
            }
            mat[i][j] = val;
        }
    }
    // HACK: Printing matrix for debugging purposes
    // printf("Printing matrix before zeroing.\n");
    // print_matrix(mat, dimension, dimension);
    // HACK:

    // Setting random matrix values to 0
    for (int l = 0; l < zeroes; l++) {
        int row, col;

        row = rand_from_range(dimension);
        col = rand_from_range(dimension);

        while (mat[row][col] == 0) {
            // Make sure to pick a new value if the one checked is already 0
            row = rand_from_range(dimension);
            col = rand_from_range(dimension);
        }

        mat[row][col] = 0;
    }

    // Create vector with random int values
    int *vec = (int *)malloc(dimension * sizeof(int));
    // HACK: Printing vector for debugging purposes

    for (int k = 0; k < dimension; k++) {
        int val = rand_from_range(100);
        vec[k] = val;
    }

    // HACK: Printing vector for debugging purposes
    printf("Vector is:\t");
    print_array(vec, dimension);
    printf("\n");
    // HACK:

    // HACK: Printing matrix for debugging purposes
    printf("Printing matrix after zeroing.\n");
    print_matrix(mat, dimension, dimension);
    // HACK:

    // Calculating number of non-zero values in the matrix
    int non_zero = total_values - zeroes;

    // Create CSR representation of sparse matrix
    timespec_get(&serial_CSRrep_start, TIME_UTC);
    CSR_t M_rep = build_CSR(mat, dimension, dimension, non_zero);
    timespec_get(&serial_CSRrep_finish, TIME_UTC);

    // Storing elapsed time
    double serial_CSR_elapsed = time_elapsed(serial_CSRrep_start, serial_CSRrep_finish);

    printf("Time of serial CSR creation: %lf\n", serial_CSR_elapsed);

    // HACK: Printing CSR representation for debugging purposes
    print_CSR(M_rep, dimension);
    // HACK:

    timespec_get(&serial_mult_start, TIME_UTC);

    // Receiving product of matrix and vector using serual execution
    int *res = (int *)malloc(dimension * sizeof(int));
    for (int repetition = 0; repetition < reps; repetition++) {
        res = mat_vec_product(mat, vec, dimension, dimension);
    }

    timespec_get(&serial_mult_finish, TIME_UTC);

    // Storing elapsed time
    double serial_mult_elapsed = time_elapsed(serial_mult_start, serial_mult_finish);

    printf("Time of serial multiplication: %lf\n", serial_mult_elapsed);

    // HACK: Printing matrix-vector product for debugging purposes
    printf("Product is:\t");
    print_array(res, dimension);
    printf("\n");
    // HACK:

    timespec_get(&serial_CSRmult_start, TIME_UTC);

    int *CSRres = CSR_mat_vec_product(M_rep, vec, dimension);

    timespec_get(&serial_CSRmult_finish, TIME_UTC);

    printf("CSR product is:\t");
    print_array(CSRres, dimension);
    printf("\n");

    // Clearing memory
    for (int row = 0; row < dimension; row++) {
        free(mat[row]);
    }
    free(mat);
    mat = NULL;
    free(vec);
    vec = NULL;
    free(res);
    res = NULL;
    free(CSRres);
    CSRres = NULL;

    return 0;
}

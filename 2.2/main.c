#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Structure used to contain a Compressed Sparse Row representation of a sparse matrix*/
typedef struct CSR {
    int **matrix;   // Sparse matrix to be represented using CSR
    int *val_array; // Array of non-zero values in the matrix
    int *col_array; // Array of column indexes of non-zero values in the matrix
    int *start_idx; // Array of indexes to the start of each row(first non-zero value)
} CSR_t;

/*Returns a random number in the range 0 - <range_max>*/
int rand_from_range(int range_max) {
    int result = rand() / (RAND_MAX / range_max + 1);
    return result;
}

/* Prints a given 2D matrix in the terminal */
void print_matrix(int **matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        printf("%d. \t", i);
        for (int j = 0; j < col; j++) {
            printf("%d \t", matrix[i][j]);
        }
        printf("\n");
    }
}

/*Builds the Compressed Sparse Row representation of a sparse matrix*/
CSR_t build_CSR(int **matrix, int row, int col, int non_zero) {
    printf("Building CSR representation.\n");
    CSR_t csr = {matrix, NULL, NULL, NULL};
    int *values = (int *)malloc(non_zero * sizeof(int));
    int *columns = (int *)malloc(non_zero * sizeof(int));
    int *row_start = (int *)malloc((row + 1) * sizeof(int));
    row_start[row + 1] = non_zero; // Last element of row_start list must contain the number of non-zero elements in the given matrix

    int list_idx = 0; // Index to use for accessing the above lists

    // Iterating through all elements in the matrix and checking for non-zero values
    for (int i = 0; i < row; i++) {
        row_start[i] = list_idx; // Storing the index of the column where a row's first non-zero element is
        for (int j = 0; j < col; j++) {
            int val = matrix[i][j];
            if (val != 0) {
                values[list_idx] = val; // Storing non-zero value
                columns[list_idx] = j;  // Storing non-zero value's column index
                list_idx++;
            }
        }
    }

    // Storing created lists in the CSR representation structure
    csr.col_array = columns;
    csr.val_array = values;
    csr.start_idx = row_start;

    return csr;
}

/*Prints a CSR representation to terminal*/
void print_CSR(CSR_t csr, int rows) {
    int nz_num = csr.start_idx[rows + 1];
    printf("List of non-zero values is:");
    for (int i = 0; i < nz_num; i++) {
        printf("%d \t", csr.val_array[i]);
    }
    printf("\n");

    printf("List of non-zero columns is:");
    for (int i = 0; i < nz_num; i++) {
        printf("%d \t", csr.col_array[i]);
    }
    printf("\n");

    printf("Starting rows are:");
    for (int i = 0; i < rows; i++) {
        printf("%d \t", csr.start_idx[i]);
    }
    printf("\n");
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

    // HACK:Printing arguments for debugging purposes
    fprintf(stderr, "Array dimension is: %d ", dimension);
    fprintf(stderr, "Zero_percentage is: %d ", zero_percentage);
    fprintf(stderr, "Reps is: %d ", reps);
    fprintf(stderr, "Threads is: %d\n", threads);
    // HACK:

    // Useful values
    int total_values = dimension * dimension;
    int zeroes = ceil(total_values * zero_percentage / 100.0);
    printf("There are %d zeroes out of %d values.\n", zeroes, total_values);

    // Create a matrix with dimensions dimension x dimension
    // Initialize all values as random integers
    // Randomly set zeroes% of all values to 0

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
    printf("Printing matrix before zeroing.\n");
    print_matrix(mat, dimension, dimension);
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

    for (int k = 0; k < dimension; k++) {
        int val = rand_from_range(100);
        vec[k] = val;
    }

    // HACK: Printing matrix for debugging purposes
    printf("Printing matrix after zeroing.\n");
    print_matrix(mat, dimension, dimension);
    // HACK:

    // Calculating number of non-zero values in the matrix
    int non_zero = total_values - zeroes;

    // Create CSR representation of sparse matrix
    CSR_t M_rep = build_CSR(mat, dimension, dimension, non_zero);

    // HACK: Printing CSR representation for debugging purposes
    print_CSR(M_rep, dimension);
    // HACK:

    return 0;
}

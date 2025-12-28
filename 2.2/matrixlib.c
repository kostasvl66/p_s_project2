#include "matrixlib.h"
#include <stdio.h>
#include <stdlib.h>

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
    printf("Values\t");
    for (int i = 0; i < nz_num; i++) {
        printf("%d \t", csr.val_array[i]);
    }
    printf("\n");

    printf("Columns\t");
    for (int i = 0; i < nz_num; i++) {
        printf("%d \t", csr.col_array[i]);
    }
    printf("\n");

    printf("Rows\t");
    for (int i = 0; i < rows; i++) {
        printf("%d \t", csr.start_idx[i]);
    }
    printf("\n");
}

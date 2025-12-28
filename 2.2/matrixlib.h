/* Structure used to contain a Compressed Sparse Row representation of a sparse matrix*/
typedef struct CSR {
    int **matrix;   // Sparse matrix to be represented using CSR
    int *val_array; // Array of non-zero values in the matrix
    int *col_array; // Array of column indexes of non-zero values in the matrix
    int *start_idx; // Array of indexes to the start of each row(first non-zero value)
} CSR_t;

/* Prints a given 2D matrix in the terminal */
void print_matrix(int **matrix, int row, int col);

/*Builds the Compressed Sparse Row representation of a sparse matrix*/
CSR_t build_CSR(int **matrix, int row, int col, int non_zero);

/*Prints a CSR representation to terminal*/
void print_CSR(CSR_t csr, int rows);

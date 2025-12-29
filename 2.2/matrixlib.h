/* Structure used to contain a Compressed Sparse Row representation of a sparse matrix*/
typedef struct CSR {
    int **matrix;   // Sparse matrix to be represented using CSR
    int *val_array; // Array of non-zero values in the matrix
    int *col_array; // Array of column indexes of non-zero values in the matrix
    int *start_idx; // Array of indexes to the start of each row(first non-zero value)
} CSR_t;

/*Builds the Compressed Sparse Row representation of a sparse matrix*/
CSR_t CSR_create(int **matrix, int row, int col, int non_zero);

/* Returns the product of multiplication between a matrix and a vector*/
int *mat_vec(int **matrix, int *vector, int row, int col);

/* Returns the product of multiplication between a matrix and a vector, using CSR representation*/
int *CSR_mat_vec(CSR_t rep, int *vec, int dimension);

/* Prints a given 2D matrix in the terminal */
void print_matrix(int **matrix, int row, int col);

/*Prints a CSR representation to terminal*/
void print_CSR(CSR_t csr, int rows);

/*Prints the elements of an integer array*/
void print_array(int *array, int len);

/*Parallel implementations using OpenMP*/

/*Builds the Compressed Sparse Row representation of a sparse matrix using parallel execution*/
CSR_t omp_build_CSR(int **matrix, int row, int col, int non_zero);

/* Returns the product of multiplication between a matrix and a vector using parallel execution*/
int *omp_mat_vec_product(int **matrix, int *vector, int row, int col);

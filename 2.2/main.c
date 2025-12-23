#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Checking for correct number of arguments
    if (argc != 5) {
        fprintf(stderr, "Program must be called as -> ./main dimension zero_percentage reps threads\n");
        fprintf(stderr, "Can also be called as -> make run D=dimension Z=zero_percentage R=reps T=threads\n");
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

    return 0;
}

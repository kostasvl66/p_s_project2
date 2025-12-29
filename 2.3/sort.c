#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define MAX_ARRAY_VALUE 1000

int n, thread_count;

// checks command usage; returns 0 on success, -1 otherwise
// if "parallel" is chosen, thread_count is at least 1, else if "serial" is chosen, thread_count gets the value 0
int parse_args(int argc, char *argv[])
{
    char usage_msg[128];
    sprintf(usage_msg, "Usage: %s <array length> {serial|parallel <thread count>}\n", argv[0]);

    if (argc != 3 && argc != 4)
    {
        fputs(usage_msg, stderr);
        return -1;
    }
        
    char *end;
    long temp_n = strtol(argv[1], &end, 10);
    if (*end != '\0' || temp_n < 1 || temp_n > INT_MAX)
    {
        fprintf(stderr, "Invalid array length.\n");
        return -1;
    }
    n = (int)temp_n;

    if (strcmp(argv[2], "serial") == 0)
    {
        if (argc != 3)
        {
            fputs(usage_msg, stderr);
            return -1;
        }

        thread_count = 0;
        return 0;
    }

    if (strcmp(argv[2], "parallel") == 0)
    {
        if (argc != 4)
        {
            fputs(usage_msg, stderr);
            return -1;
        }

        long temp_thread_count = strtol(argv[3], &end, 10);
        if (*end != '\0' || temp_thread_count < 1 || temp_thread_count > INT_MAX)
        {
            fprintf(stderr, "Invalid thread count.\n");
            return -1;
        }
        
        thread_count = (int)temp_thread_count;
        return 0;
    }

    fputs(usage_msg, stderr);
    return -1;
}

// calculates time difference between start and end with nsec precision
double elapsed(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

// allocates and initializes an array of length with random integers in [0, MAX_ARRAY_VALUE]
// returns pointer to the array, or NULL if allocation was unsuccessful
// caller is responsible for freeing the returned pointer
// srand() should be used before this call
int *create_random_arr(int length)
{
    int *new_arr = malloc(length * sizeof(int));
    if (!new_arr)
        return NULL;

    for (int i = 0; i < length; i++)
        new_arr[i] = rand() % (MAX_ARRAY_VALUE + 1);

    return new_arr;
}

// returns if the given array is sorted
int is_sorted(int *arr, int length)
{
    for (int i = 1; i < length; i++)
        if (arr[i] < arr[i - 1])
            return 0;
    return 1;
}

// merges the [lower, mid] and [mid + 1, higher] portions of an array arr
// returns 0 on success, -1 on failure
int merge(int *arr, int lower, int mid, int higher)
{
	int length1 = mid - lower + 1;
	int length2 = higher - mid;

    int *arr1 = NULL;
    int *arr2 = NULL;
	arr1 = malloc(length1 * sizeof(int));
	arr2 = malloc(length2 * sizeof(int));
    if (!arr1 || !arr2)
    {
        free(arr1);
        free(arr2);
        return -1;
    }

	for (int i = 0; i < length1; i++)
		arr1[i] = arr[lower + i];
	for (int i = 0; i < length2; i++)
		arr2[i] = arr[mid + 1 + i];
	
	int i = 0, j = 0, k = 0;
	while (i < length1 && j < length2)
		if (arr1[i] < arr2[j])
			arr[lower + k++] = arr1[i++];
		else
			arr[lower + k++] = arr2[j++];

	while (i < length1)
		arr[lower + k++] = arr1[i++];
	while (j < length2)
		arr[lower + k++] = arr2[j++];

	free(arr1);
    free(arr2);
    return 0;
}

// serial algorithm for merge sort; lower and higher are inclusive
// returns 0 on success, -1 on failure
int mergesort(int *arr, int lower, int higher)
{
    if (lower >= higher)
        return 0;

    int mid = (lower + higher) / 2;
    if (mergesort(arr, lower, mid) == -1)
        return -1;
    if (mergesort(arr, mid + 1, higher) == -1)
        return -1;

    if (merge(arr, lower, mid, higher) == -1)
        return -1;

    return 0;
}

// parallel algorithm for merge sort; lower and higher are inclusive
// returns 0 on success, -1 on failure
int mergesort_threaded_internal(int *arr, int lower, int higher, int thread_count)
{
    int success = 1;

    if (lower < higher)
    {
        int mid = (lower + higher) / 2;
        
        #pragma omp task if(higher - lower > 1000) firstprivate(lower, mid)
        if (mergesort_threaded_internal(arr, lower, mid, thread_count) == -1)
            success = 0;

        #pragma omp task if(higher - lower > 1000) firstprivate(mid, higher)
        if (mergesort_threaded_internal(arr, mid + 1, higher, thread_count) == -1)
            success = 0;

        #pragma omp taskwait
        if (success)
            if (merge(arr, lower, mid, higher) == -1)
                success = 0;
    }

    return (success ? 0 : -1);
}

// parallel algorithm for merge sort; lower and higher are inclusive
// returns 0 on success, -1 on failure
int mergesort_threaded(int *arr, int length, int thread_count)
{
    int ret_val;

    #pragma omp parallel num_threads(thread_count) \
        default(none) shared(arr, length, thread_count, ret_val)
    #pragma omp single
    ret_val = mergesort_threaded_internal(arr, 0, length - 1, thread_count);

    return ret_val;
}

int main(int argc, char *argv[])
{
    if (parse_args(argc, argv) == -1)
        return 1;

    srand(123456);
    int *arr = create_random_arr(n);
    if (!arr)
        return 1;

    struct timespec start, end;
    timespec_get(&start, TIME_UTC);

    int err;
    if (thread_count == 0)
        err = mergesort(arr, 0, n - 1);
    else
        err = mergesort_threaded(arr, n, thread_count);

    if (err == -1)
    {
        free(arr);
        return 1;
    }

    timespec_get(&end, TIME_UTC);

    if (is_sorted(arr, n))
        printf("Array is sorted.\n");
    else
        printf("Array is not sorted.\n");
    
    if (thread_count == 0)
        printf("Serial algorithm: %.6f sec\n", elapsed(start, end));
    else
        printf("Parallel algorithm: %.6f sec\n", elapsed(start, end));

    free(arr);
    return 0;
}
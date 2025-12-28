#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

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

int main(int argc, char *argv[])
{
    if (parse_args(argc, argv) == -1)
        return -1;

    

    return 0;
}
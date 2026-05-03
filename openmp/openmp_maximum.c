#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 20000000
#define MAX_LEN 1024

char *lines[MAX_LINES];
int results[MAX_LINES];
int total_lines = 0;

void count_array()
{
    #pragma omp parallel for // iterate loop among multiple threads in parallel
    for (int i = 0; i < total_lines; i++)
    {
        // Track maximum ASCII value
        int max = -1;
        // Iterate over characters in the string
        for (int j = 0; lines[i][j] != '\0'; j++)
        {
            // Skip newline character           
            if (lines[i][j] == '\n') continue;
            // Compare ASCII values
            if ((int)lines[i][j] > max)
            {
                max = (int)lines[i][j];
            }
        }
        // Write result to global array
        results[i] = max;
    }
}

// Print results
void print_results()
{
    for (int i = 0; i < total_lines; i++)
    {
        printf("%d: %d\n", i, results[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    // stores number of threads
    int num_threads;

    // Validate number of threads
    if (sscanf(argv[1], "%d", &num_threads) != 1 || num_threads <= 0)
    {
        printf("Invalid number of threads.\n");
        return 1;
    }

    // Set number of OpenMP threads
    omp_set_num_threads(num_threads);

    // Open input file
    FILE *fp = fopen("/homes/eyv/cis520/wiki_dump.txt", "r");

    // If file cannot be opened, print error and return 1
    if (fp == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // stack buffer for reading lines
    char buffer[MAX_LEN];

    // Read file line by line
    while (fgets(buffer, sizeof(buffer), fp))
    {
        lines[total_lines] = strdup(buffer);
        total_lines++;
    }

    // close file
    fclose(fp);

    // find max ASCII values
    count_array();

    // Free allocated memory for each line
    for (int i = 0; i < total_lines; i++)
    {
        free(lines[i]);
    }

    return 0;
}
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 20000000
#define MAX_LEN 1024

char *lines[MAX_LINES];
int results[MAX_LINES];
int local_results[MAX_LINES];

int total_lines = 0;
int rank;
int num_processes;

void count_array()
{
    // Number of lines per process
    int chunk = total_lines / num_processes;
    // Process starting index
    int startPos = rank * chunk;
    // Last one gets leftover lines
    int endPos = (rank == num_processes - 1) ? total_lines : startPos + chunk;

    for (int i = startPos; i < endPos; i++)
    {
        // Track max ASCII value
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
        // Write result to local array
        local_results[i] = max;
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
    MPI_Init(&argc, &argv);

    // Get current process's rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    // Open input file
    FILE *fp = fopen("/homes/eyv/cis520/wiki_dump.txt", "r");

    // If file cannot be opened, print error and return 1
    if (fp == NULL)
    {
        printf("Rank %d could not open file.\n", rank);
        MPI_Finalize();
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
    
    // Initialize arrays to 0
    for (int i = 0; i < total_lines; i++)
    {
        local_results[i] = 0;
        results[i] = 0;
    }

    // Find max ASCII for that process's lines
    count_array();

    // Put all results into rank 0
    MPI_Reduce(local_results, results, total_lines, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Free memory for each line
    for (int i = 0; i < total_lines; i++)
    {
        free(lines[i]);
    }

    // Finalize MPI environment
    MPI_Finalize();

    return 0;
}
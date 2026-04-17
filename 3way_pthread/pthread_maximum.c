#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 4
#define MAX_LINES 20000000
#define MAX_LEN 1024

char *lines[MAX_LINES];
int results[MAX_LINES];
int total_lines = 0;

void *count_array(void *arg) {
    // Thread ID passed from main()
    int thread_id = (int)(long)arg;
    // Divide work among threads
    int chunk = total_lines / NUM_THREADS;
    // Compute assigned range for thread
    int startPos = thread_id * chunk;
    // Last one gets leftover lines
    int endPos = (thread_id == NUM_THREADS - 1) ? total_lines : startPos + chunk;

    for (int i = startPos; i < endPos; i++) {
        // Track maximum ASCII value
        int max = -1; 
        // Iterate over characters in the string
        for (int j = 0; lines[i][j] != '\0'; j++) {
            // Skip newline character
            if (lines[i][j] == '\n') continue;
            // Compare ASCII values
            if ((int)lines[i][j] > max) {
                max = (int)lines[i][j];
            }
        }
        // Write result to global array
        results[i] = max;
    }
    // Terminate thread
    pthread_exit(NULL);
}

// Print results
void print_results() {
    for (int i = 0; i < total_lines; i++) {
        printf("%d: %d\n", i, results[i]);
    }
}

int main() {
    // Open file
    FILE *fp = fopen("/homes/eyv/cis520/wiki_dump.txt", "r");
    // Stack buffer for reading lines
    char buffer[MAX_LEN];
    // Read file line by line
    while (fgets(buffer, sizeof(buffer), fp)) {
        lines[total_lines] = strdup(buffer);
        total_lines++;
    }

    fclose(fp);
    // Thread handles
    pthread_t threads[NUM_THREADS];
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, count_array, (void *)(long)i)) {
            printf("Error creating thread\n");
            return 1;
        }
    }
    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // Output results
    print_results();
    return 0;
}

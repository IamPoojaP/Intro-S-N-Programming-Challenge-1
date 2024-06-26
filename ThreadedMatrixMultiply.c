// Include necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define constants for matrix size and number of threads
#define MATRIX_SIZE 3
#define NUM_THREADS 3

// Declare global matrices and variables
int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];
int calculationCount = 0; // Initialize calculationCount
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Initialize mutex

// Define a struct to hold thread data
typedef struct {
    int row;
    int col;
} thread_data_t;

// Function to perform matrix multiplication in a thread
void *multiply(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int localCount = 0;

    // Perform matrix multiplication
    for (int i = 0; i < MATRIX_SIZE; i++) {
        resultMatrix[data->row][data->col] += matrixA[data->row][i] * matrixB[i][data->col];
        localCount++; // Increment localCount for each calculation
    }

    // Lock mutex before updating calculationCount
    pthread_mutex_lock(&mutex);
    calculationCount += localCount; // Update calculationCount
    pthread_mutex_unlock(&mutex); // Unlock mutex

    pthread_exit(NULL);
}

// Main function
int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    // Initialize matrices A and B
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // Create threads to compute the result matrix
    int thread_count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_count].row = i;
            thread_data[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, multiply, (void *)&thread_data[thread_count]);
            thread_count++;

            if (thread_count >= NUM_THREADS)
                thread_count = 0;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    // Print the calculation count
    printf("Total number of calculations: %d\n", calculationCount);

    return 0;
}

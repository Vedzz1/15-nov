#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int **A, **B, **Add, **Sub, **Mul;
int size;

void* addMatrices(void* arg) {
    int threadId = *((int*)arg);
    for (int i = threadId; i < size; i += 2) {
        for (int j = 0; j < size; j++) {
            Add[i][j] = A[i][j] + B[i][j];
        }
    }
    return NULL;
}

void* subtractMatrices(void* arg) {
    int threadId = *((int*)arg);
    for (int i = threadId; i < size; i += 2) {
        for (int j = 0; j < size; j++) {
            Sub[i][j] = A[i][j] - B[i][j];
        }
    }
    return NULL;
}

void* multiplyMatrices(void* arg) {
    int threadId = *((int*)arg);
    for (int i = threadId; i < size; i += 2) {
        for (int j = 0; j < size; j++) {
            Mul[i][j] = 0; // Initialize result
            for (int k = 0; k < size; k++) {
                Mul[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

void printMatrix(int** matrix) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int** allocateMatrix() {
    int** matrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        matrix[i] = (int*)malloc(size * sizeof(int));
    }
    return matrix;
}

void initializeMatrix(int** matrix) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }
}

void freeMatrix(int** matrix) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    pthread_t addThreads[2], subThreads[2], mulThreads[2];
    int threadIds[2] = {0, 1};

    // Take size of the matrix from user
    printf("Enter the size of the matrices: ");
    scanf("%d", &size);

    // Allocate memory for matrices
    A = allocateMatrix();
    B = allocateMatrix();
    Add = allocateMatrix();
    Sub = allocateMatrix();
    Mul = allocateMatrix();

    // Taking user input for Matrix A
    printf("Enter elements of Matrix A (%dx%d):\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    // Taking user input for Matrix B
    printf("Enter elements of Matrix B (%dx%d):\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf("%d", &B[i][j]);
        }
    }

    // Initialize the result matrices
    initializeMatrix(Add);
    initializeMatrix(Sub);
    initializeMatrix(Mul);

    // Create threads for addition, subtraction, and multiplication
    for (int i = 0; i < 2; i++) {
        pthread_create(&addThreads[i], NULL, addMatrices, &threadIds[i]);
        pthread_create(&subThreads[i], NULL, subtractMatrices, &threadIds[i]);
        pthread_create(&mulThreads[i], NULL, multiplyMatrices, &threadIds[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(addThreads[i], NULL);
        pthread_join(subThreads[i], NULL);
        pthread_join(mulThreads[i], NULL);
    }

    // Print the results
    printf("\nAddition of Matrix A and B:\n");
    printMatrix(Add);

    printf("\nSubtraction of Matrix A and B:\n");
    printMatrix(Sub);

    printf("\nMultiplication of Matrix A and B:\n");
    printMatrix(Mul);

    // Free dynamically allocated memory
    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(Add);
    freeMatrix(Sub);
    freeMatrix(Mul);

    return 0;
}


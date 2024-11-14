#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function

// Shared Data Structure
typedef struct {
    int counter;                // Counter for writer
    int readerCount;            // Counter for readers
    sem_t mutex;                // Mutex for protecting shared data
    sem_t writeSemaphore;       // Semaphore to control writer access
    int writingComplete;        // Flag to indicate writing completion
} SharedData;

// Writer function
void* writer_function(void* arg) {
    SharedData* sharedData = (SharedData*)arg;
    int limit = sharedData->counter;

    int flag = 1;
    while (flag) {
        sem_wait(&sharedData->writeSemaphore); // Acquire the semaphore for writing
        sem_wait(&sharedData->mutex); // Acquire the mutex to access shared data

        sharedData->counter++;
        printf("Writer has access.\n");
        printf("Thread ID = %lu\n", pthread_self());
        printf("Counter = %d\n", sharedData->counter);

        // Check if it has reached the limit
        if (sharedData->counter >= limit) {
            flag = 0;
            sharedData->writingComplete = 1; // Set the writing complete flag
        }

        sem_post(&sharedData->mutex); // Release the mutex
        sem_post(&sharedData->writeSemaphore); // Release the semaphore

        sleep(1); // Simulate work
    }
    return NULL;
}

// Reader function
void* reader_function(void* arg) {
    SharedData* sharedData = (SharedData*)arg;

    while (1) {
        sem_wait(&sharedData->mutex); // Acquire the mutex to access shared data
        sharedData->readerCount++;
        
        if (sharedData->readerCount == 1) {
            sem_wait(&sharedData->writeSemaphore); // First reader locks the writer
        }

        // Check if writing is complete, if yes, we exit
        if (sharedData->writingComplete) {
            sharedData->readerCount--;
            if (sharedData->readerCount == 0) {
                sem_post(&sharedData->writeSemaphore); // Last reader unlocks the writer
            }
            sem_post(&sharedData->mutex); // Release the mutex
            break; // Exit the loop
        }

        printf("Reader has access.\n");
        printf("Thread ID = %lu\n", pthread_self());
        printf("Counter = %d\n", sharedData->counter);

        sharedData->readerCount--;
        if (sharedData->readerCount == 0) {
            sem_post(&sharedData->writeSemaphore); // Last reader unlocks the writer
        }

        sem_post(&sharedData->mutex); // Release the mutex
        sleep(1); // Simulate work
    }
    return NULL;
}

int main() {
    int numberOfReaders;
    int writerLimit;

    // Take user input for the number of readers and the limit for the writer
    printf("Enter the number of readers: ");
    scanf("%d", &numberOfReaders);

    printf("Enter the limit for the writer: ");
    scanf("%d", &writerLimit);

    // Initialize shared data
    SharedData sharedData;
    sharedData.counter = 0;
    sharedData.readerCount = 0;
    sharedData.writingComplete = 0;
    sem_init(&sharedData.mutex, 0, 1); // Initialize the mutex
    sem_init(&sharedData.writeSemaphore, 0, 1); // Initialize the write semaphore

    // Create writer thread
    pthread_t writer;
    pthread_create(&writer, NULL, writer_function, (void*)&sharedData);

    // Create reader threads
    pthread_t readers[numberOfReaders];
    for (int i = 0; i < numberOfReaders; i++) {
        pthread_create(&readers[i], NULL, reader_function, (void*)&sharedData);
    }

    // Wait for the writer thread to finish
    pthread_join(writer, NULL);

    // Wait for all reader threads to finish
    for (int i = 0; i < numberOfReaders; i++) {
        pthread_join(readers[i], NULL);
    }

    // Clean up semaphores
    sem_destroy(&sharedData.mutex);
    sem_destroy(&sharedData.writeSemaphore);

    return 0;
}


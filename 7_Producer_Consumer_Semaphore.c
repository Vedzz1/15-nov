#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // For sleep function
#include <time.h>   // For random number generation

#define MAX_BUFFER_SIZE 100

int buffer[MAX_BUFFER_SIZE]; // Shared buffer
int bufferSize; // Maximum buffer size
int in = 0; // Index for producer to place items
int out = 0; // Index for consumer to take items
int maxItems; // Max items to produce/consume
int producedCount = 0; // Count of produced items
int consumedCount = 0; // Count of consumed items

// Semaphores for synchronization
sem_t empty; // Counts empty buffer slots
sem_t full;  // Counts full buffer slots
pthread_mutex_t mutex; // Mutex for critical section (protecting the buffer)

void* producer(void* arg) {
    while (producedCount < maxItems) {
        int item = rand() % 100; // Produce a random item
        sem_wait(&empty); // Wait if there are no empty slots
        pthread_mutex_lock(&mutex); // Lock the buffer

        if (producedCount < maxItems) {
            buffer[in] = item; // Add the item to the buffer
            printf("Produced: %d\n", item);
            in = (in + 1) % bufferSize; // Circular buffer
            producedCount++;
        }

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&full); // Signal that there's a new full slot
        sleep(1); // Simulate production time
    }
    return NULL;
}

void* consumer(void* arg) {
    while (consumedCount < maxItems) {
        sem_wait(&full); // Wait if there are no full slots
        pthread_mutex_lock(&mutex); // Lock the buffer

        if (consumedCount < producedCount) {
            int item = buffer[out]; // Take the item from the buffer
            printf("Consumed: %d\n", item);
            out = (out + 1) % bufferSize; // Circular buffer
            consumedCount++;
        }

        pthread_mutex_unlock(&mutex); // Unlock the buffer
        sem_post(&empty); // Signal that there's a new empty slot
        sleep(1); // Simulate consumption time
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // Seed random number generator
    int numProducers, numConsumers;

    // Input: Number of producers
    printf("Enter the number of producers: ");
    scanf("%d", &numProducers);

    // Input: Number of consumers
    printf("Enter the number of consumers: ");
    scanf("%d", &numConsumers);

    // Input: Size of the buffer
    printf("Enter the size of the buffer: ");
    scanf("%d", &bufferSize);

    // Input: Maximum items to produce and consume
    printf("Enter the maximum number of items to produce and consume: ");
    scanf("%d", &maxItems);

    // Initialize semaphores and mutex
    sem_init(&empty, 0, bufferSize); // Buffer initially has `bufferSize` empty slots
    sem_init(&full, 0, 0); // No full slots at the beginning
    pthread_mutex_init(&mutex, NULL);

    pthread_t producers[numProducers], consumers[numConsumers];

    // Create and start producer threads
    for (int i = 0; i < numProducers; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    // Create and start consumer threads
    for (int i = 0; i < numConsumers; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    // Wait for all producers to finish
    for (int i = 0; i < numProducers; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for all consumers to finish
    for (int i = 0; i < numConsumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}


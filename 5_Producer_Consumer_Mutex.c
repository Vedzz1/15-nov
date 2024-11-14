#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep function
#include <time.h>   // For random number generation

#define MAX_BUFFER_SIZE 100

pthread_mutex_t bufferMutex; // Mutex to ensure mutual exclusion
pthread_cond_t bufferNotFull; // Condition variable to signal buffer is not full
pthread_cond_t bufferNotEmpty; // Condition variable to signal buffer is not empty

int buffer[MAX_BUFFER_SIZE]; // Shared buffer
int bufferSize; // Maximum buffer size
int in = 0; // Index for producer to place items
int out = 0; // Index for consumer to take items
int itemCount = 0; // Count of items currently in the buffer
int maxItems; // Max items to produce/consume
int producedCount = 0; // Count of total produced items
int consumedCount = 0; // Count of total consumed items

void* producer(void* arg) {
    while (producedCount < maxItems) {
        int item = rand() % 100; // Produce a random item
        pthread_mutex_lock(&bufferMutex); // Lock the buffer

        while (itemCount == bufferSize) {
            // Buffer is full, wait for space
            pthread_cond_wait(&bufferNotFull, &bufferMutex);
        }

        if (producedCount < maxItems) {
            buffer[in] = item; // Put the item into the buffer
            printf("Produced: %d\n", item);
            in = (in + 1) % bufferSize; // Circular buffer
            itemCount++;
            producedCount++;
        }

        // Signal that the buffer is not empty (an item is available for consumption)
        pthread_cond_signal(&bufferNotEmpty);
        pthread_mutex_unlock(&bufferMutex); // Unlock the buffer
        sleep(1); // Simulate production time
    }
    return NULL;
}

void* consumer(void* arg) {
    while (consumedCount < maxItems) {
        pthread_mutex_lock(&bufferMutex); // Lock the buffer

        while (itemCount == 0) {
            // Buffer is empty, wait for items
            pthread_cond_wait(&bufferNotEmpty, &bufferMutex);
        }

        if (consumedCount < producedCount) {
            int item = buffer[out]; // Get the item from the buffer
            printf("Consumed: %d\n", item);
            out = (out + 1) % bufferSize; // Circular buffer
            itemCount--;
            consumedCount++;
        }

        // Signal that the buffer is not full (a slot is available for production)
        pthread_cond_signal(&bufferNotFull);
        pthread_mutex_unlock(&bufferMutex); // Unlock the buffer
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

    // Initialize mutex and condition variables
    pthread_mutex_init(&bufferMutex, NULL);
    pthread_cond_init(&bufferNotFull, NULL);
    pthread_cond_init(&bufferNotEmpty, NULL);

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

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&bufferMutex);
    pthread_cond_destroy(&bufferNotFull);
    pthread_cond_destroy(&bufferNotEmpty);

    return 0;
}


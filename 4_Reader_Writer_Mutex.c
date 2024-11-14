#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // For sleep function

// Shared Data Structure
typedef struct {
    int counter;                // Counter for writer
    int readerCount;            // Counter for readers
    pthread_mutex_t lock;       // Mutex for synchronizing access
    int writingComplete;        // Flag to indicate writing completion
} SharedData;

// Writer function
void* writer_function(void* arg) {
    SharedData* sharedData = (SharedData*)arg;
    int limit = sharedData->counter; // Get the limit from sharedData

    int flag = 1;
    while (flag) {
        pthread_mutex_lock(&sharedData->lock); // Acquire the lock
        
        sharedData->counter++;
        printf("Writer has access.\n");
        printf("Thread ID = %ld\n", pthread_self());
        printf("Counter = %d\n", sharedData->counter);
        
        if (sharedData->counter >= limit) {
            flag = 0;
            sharedData->writingComplete = 1; // Set the writing complete flag
        }
        pthread_mutex_unlock(&sharedData->lock); // Release the lock
        sleep(1); // Simulate work
    }
    return NULL;
}

// Reader function
void* reader_function(void* arg) {
    SharedData* sharedData = (SharedData*)arg;

    while (1) {
        pthread_mutex_lock(&sharedData->lock); // Acquire the lock

        sharedData->readerCount++;
        if (sharedData->readerCount == 1) {
            // First reader can lock resources here if needed
        }

        printf("Reader (Thread ID: %ld) is entering critical section.\n", pthread_self());
        printf("Reader (Thread ID: %ld) is reading. Counter = %d\n", pthread_self(), sharedData->counter);
        
        sharedData->readerCount--;
        printf("Reader (Thread ID: %ld) is leaving critical section.\n", pthread_self());

        if (sharedData->readerCount == 0) {
            // Last reader unlocks the resource
        }

        // Check if the writer is done
        int writer_done = sharedData->writingComplete;
        pthread_mutex_unlock(&sharedData->lock); // Release the lock

        if (writer_done) {
            break; // Exit if writing is complete
        }

        sleep(1); // Simulate work
    }
    return NULL;
}

int main() {
    int numberOfReaders;
    int limit;

    // Prompt for number of readers
    printf("Enter the number of readers: ");
    scanf("%d", &numberOfReaders);

    // Prompt for the limit for the writer
    printf("Enter the limit for the writer: ");
    scanf("%d", &limit);

    // Initialize shared data
    SharedData sharedData;
    sharedData.counter = 0;
    sharedData.readerCount = 0;
    sharedData.writingComplete = 0;
    pthread_mutex_init(&sharedData.lock, NULL); // Initialize the mutex

    // Create writer thread
    pthread_t writer;
    sharedData.counter = limit; // Store the limit in shared data for the writer
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

    // Destroy the mutex
    pthread_mutex_destroy(&sharedData.lock);
    return 0;
}


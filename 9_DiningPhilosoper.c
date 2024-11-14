#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t chopsticks[NUM_PHILOSOPHERS]; // Semaphore for each chopstick

void* philosopher(void* num) {
    int philosopher = *(int*)num;
    int left = philosopher;                       // Left chopstick
    int right = (philosopher + 1) % NUM_PHILOSOPHERS; // Right chopstick

    while (1) {
        printf("Philosopher %d is thinking.\n", philosopher);
        sleep(1); // Simulate thinking

        // To avoid deadlock, ensure philosophers pick up the lower-numbered chopstick first
        if (philosopher % 2 == 0) {
            sem_wait(&chopsticks[left]); // Pick up left chopstick
            sem_wait(&chopsticks[right]); // Pick up right chopstick
        } else {
            sem_wait(&chopsticks[right]); // Pick up right chopstick
            sem_wait(&chopsticks[left]); // Pick up left chopstick
        }

        // Eating
        printf("Philosopher %d is eating.\n", philosopher);
        sleep(1); // Simulate eating

        // Put down chopsticks
        sem_post(&chopsticks[left]);
        sem_post(&chopsticks[right]);

        printf("Philosopher %d finished eating and put down chopsticks.\n", philosopher);
        sleep(1); // Give time for others to proceed
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize semaphores for each chopstick
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

    // Create threads for each philosopher
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Join threads (wait for all philosopher threads to finish)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}


gcc dining_philosophers.c -o dining_philosophers -lpthread
./dining_philosophers

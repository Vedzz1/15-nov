#include <iostream>
#include <thread>
#include <semaphore.h>

const int NUM_PHILOSOPHERS = 5;

// Semaphores to control access to the forks
sem_t forks[NUM_PHILOSOPHERS];

// Function that represents a philosopher's behavior
void philosopher(int id) {
    while (true) {
        // Think
        std::cout << "Philosopher " << id << " is thinking." << std::endl;

        // Pick up left fork
        sem_wait(&forks[id]);

        // Pick up right fork (mod to handle the last philosopher)
        sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);

        // Eat
        std::cout << "Philosopher " << id << " is eating." << std::endl;

        // Put down forks
        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % NUM_PHILOSOPHERS]);
    }
}

int main() {
    // Initialize semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    std::thread philosophers[NUM_PHILOSOPHERS];
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i] = std::thread(philosopher, i);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].join();
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
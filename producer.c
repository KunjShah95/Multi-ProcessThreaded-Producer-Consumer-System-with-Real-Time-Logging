/* ----------- producer.c ----------- */
#include "shared.h"

SharedData *shared_mem;
sem_t *sem_empty, *sem_full, *sem_mutex;

void* producer_thread(void *arg) {
    while(1) {
        int item = rand() % 1000;
        
        sem_wait(sem_empty);
        sem_wait(sem_mutex);

        // Critical Section
        shared_mem->buffer[shared_mem->in] = item;
        shared_mem->in = (shared_mem->in + 1) % BUFFER_SIZE;
        
        log_entry("PRODUCED", item);
        
        sem_post(sem_mutex);
        sem_post(sem_full);
        
        sleep(1);
    }
}

int main() {
    // Attach shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    shared_mem = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Open semaphores
    sem_empty = sem_open("/empty", 0);
    sem_full = sem_open("/full", 0);
    sem_mutex = sem_open("/mutex", 0);

    // Create threads
    pthread_t threads[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, producer_thread, NULL);
    }

    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
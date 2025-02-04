/* ----------- consumer.c ----------- */
#include "shared.h"

SharedData *shared_mem;
sem_t *sem_empty, *sem_full, *sem_mutex;

void* consumer_thread(void *arg) {
    while(1) {
        sem_wait(sem_full);
        sem_wait(sem_mutex);

        // Critical Section
        int item = shared_mem->buffer[shared_mem->out];
        shared_mem->out = (shared_mem->out + 1) % BUFFER_SIZE;
        
        log_entry("CONSUMED", item);
        
        sem_post(sem_mutex);
        sem_post(sem_empty);
        
        sleep(2);
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
        pthread_create(&threads[i], NULL, consumer_thread, NULL);
    }

    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
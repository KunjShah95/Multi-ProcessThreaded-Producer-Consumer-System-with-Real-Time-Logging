/* ----------- main.c (Controller Process) ----------- */
#include "shared.h"

SharedData *shared_mem;
const char *SHM_NAME = "/prod_cons_shm";
sem_t *sem_empty, *sem_full, *sem_mutex;

int main() {
    // Signal handling
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    // Create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    shared_mem = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize shared data
    shared_mem->in = 0;
    shared_mem->out = 0;

    // Create semaphores
    sem_empty = sem_open("/empty", O_CREAT, 0666, BUFFER_SIZE);
    sem_full = sem_open("/full", O_CREAT, 0666, 0);
    sem_mutex = sem_open("/mutex", O_CREAT, 0666, 1);

    // Fork producer and consumer
    pid_t producer = fork();
    if (producer == 0) {
        execl("./producer", "producer", NULL);
    }
    
    pid_t consumer = fork();
    if (consumer == 0) {
        execl("./consumer", "consumer", NULL);
    }

    // Wait for children
    wait(NULL);
    wait(NULL);

    cleanup(0);
    return 0;
}

void cleanup(int sig) {
    munmap(shared_mem, sizeof(SharedData));
    shm_unlink(SHM_NAME);
    sem_unlink("/empty");
    sem_unlink("/full");
    sem_unlink("/mutex");
    exit(0);
}
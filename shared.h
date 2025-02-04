/* ----------- shared.h ----------- */
#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>

#define BUFFER_SIZE 10
#define MAX_THREADS 3

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    sem_t *empty;
    sem_t *full;
    sem_t *mutex;
} SharedData;

void log_entry(const char *action, int item);
void cleanup(int sig);

#endif
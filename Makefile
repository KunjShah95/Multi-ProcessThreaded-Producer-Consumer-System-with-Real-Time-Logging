CC = gcc
CFLAGS = -Wall -lpthread -lrt

all: controller producer consumer

controller: main.c log.c
    $(CC) $(CFLAGS) $^ -o controller

producer: producer.c log.c
    $(CC) $(CFLAGS) $^ -o producer

consumer: consumer.c log.c
    $(CC) $(CFLAGS) $^ -o consumer

clean:
    rm -f controller producer consumer operations.log
    rm -f /dev/shm/*
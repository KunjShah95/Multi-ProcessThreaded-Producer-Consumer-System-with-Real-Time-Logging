/* ----------- log.c ----------- */
#include "shared.h"

void log_entry(const char *action, int item) {
    int fd = open("operations.log", O_WRONLY | O_APPEND | O_CREAT, 0644);
    char buffer[50];
    int len = sprintf(buffer, "[%ld] %s %d\n", time(NULL), action, item);
    write(fd, buffer, len);
    close(fd);
}
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"

char *read_string(int fd) {
    int len;
    if (read(fd, &len, sizeof(int)) != sizeof(int)) return NULL;
    char *result = malloc(len+1);
    CHECKTRUEMSG(len+1 == read(fd, result, len+1), "could not read correct number of bytes");
    return result;
}

void create_pipes() {
    printf("creating pipes...\n");
    if (mkfifo(PTC_NAME, 0644) == -1 && errno != EEXIST) CHECKTRUEMSG(0, "could not create "PTC_NAME);
    if (mkfifo(CTP_NAME, 0644) == -1 && errno != EEXIST) CHECKTRUEMSG(0, "could not create "CTP_NAME);
}

void remove_pipes() {
    printf("removing pipes...\n");
    if (unlink(PTC_NAME) == -1 && errno != ENOENT) CHECKTRUEMSG(0, "could not remove "PTC_NAME);
    if (unlink(CTP_NAME) == -1 && errno != ENOENT) CHECKTRUEMSG(0, "could not remove "CTP_NAME);
}

void write_string_len(int fd, char *str, int len) {
    CHECKNEG1MSG(write(fd, &len, sizeof(int)), "could not write int");
    CHECKNEG1MSG(write(fd, str, len+1), "could not write string");
}

void write_string(int fd, char *str) {
    write_string_len(fd, str, strlen(str));
}

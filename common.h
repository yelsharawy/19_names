#ifndef COMMON_H
#define COMMON_H

#define CHECKTRUEMSG(value, msg) do {       \
    if (!(value)) {                         \
        printf(msg ": %m\n");               \
        exit(-1);                           \
    }                                       \
} while(0);

#define CHECKNEG1MSG(value, msg) CHECKTRUEMSG((value) != (typeof(value))-1, msg)

#define PTC_NAME "./ptc_pipe"
#define CTP_NAME "./ctp_pipe"

char *read_string(int fd);

void write_string_len(int fd, char *str, int len);

void write_string(int fd, char *str);

void create_pipes();

void remove_pipes();

#endif // COMMON_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "common.h"

void transform_string(char *str) {
    // ROT13 cuz why not
    // tried to optimize as much as possible just for fun
    // didn't actually test the speed, so i don't know how much using bit operations helped
    // as opposed to regular math and comparisons
    while (*str) {
        unsigned char div, rem;
        if ((div = *str >> 5) & 2 && (rem = ((*str) & 31) - 1) < 26)
            *str = (div << 5) | ((rem + 13) % 26 + 1);
        str++;
    }
}

int worker(int ptc_read, int ctp_write) {
    while (1) {
        char *received = read_string(ptc_read);
        if (received == NULL) {
            printf("No string received from interface. Will exit now.\n");
            return 0;
        }
        // useful printing to see what's happening (mainly for debugging)
        // printf("Received from interface: \"%s\"\n", received);
        transform_string(received);
        // printf("Sending to interface: \"%s\"\n", received);
        write_string(ctp_write, received);
        free(received);
    }
}

int main() {
    create_pipes();
    
    printf("opening pipes...\n");
    int ptc_read = open(PTC_NAME, O_RDONLY);
    CHECKNEG1MSG(ptc_read, "could not open "PTC_NAME" for reading");
    int ctp_write = open(CTP_NAME, O_WRONLY);
    CHECKNEG1MSG(ctp_write, "could not open "CTP_NAME" for writing");
    
    remove_pipes();
    
    int retval = worker(ptc_read, ctp_write);
    
    printf("closing pipes...\n");
    close(ptc_read);
    close(ctp_write);
    
    return retval;
}
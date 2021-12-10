#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "common.h"

int interface(int ptc_write, int ctp_read) {
    while (1) {
        char *line = NULL;
        unsigned long allocatedLen = 0;
        printf("Input string to process: ");
        fflush(stdout);
        int actualLen = getline(&line, &allocatedLen, stdin); // allocates `line` for me :)
        if (actualLen == -1) {
            printf("\nCould not read from stdin. Will exit now.\n");
            free(line);
            return 0;
        }
        if (line[actualLen-1] == '\n') line[actualLen-1] = 0;
        else printf("\n");
        // * known bug: this seems to terminate the program if the other end has closed
        // not sure why this happens? i don't even get a segfault
        write_string_len(ptc_write, line, actualLen);
        free(line);
        
        char *received = read_string(ctp_read);
        if (received == NULL) {
            printf("No string received from worker - killed early? Will exit now.\n");
            return -1;
        }
        printf("Received from worker: \"%s\"\n", received);
        free(received);
    }
}

int main() {
    create_pipes();
    
    printf("opening pipes...\n");
    int ptc_write = open(PTC_NAME, O_WRONLY);
    CHECKNEG1MSG(ptc_write, "could not open "PTC_NAME" for writing");
    int ctp_read = open(CTP_NAME, O_RDONLY);
    CHECKNEG1MSG(ctp_read, "could not open "CTP_NAME" for reading");
    
    remove_pipes();
    
    int retval = interface(ptc_write, ctp_read);
    
    printf("closing pipes...\n");
    close(ptc_write);
    close(ctp_read);
    
    return retval;
}
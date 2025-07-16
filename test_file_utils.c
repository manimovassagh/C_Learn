// test_file_utils.c - Simple test for file_utils

#include "file_utils.h"
#include <stdio.h>

int main() {
    int fd = open_file("file1.txt");
    if (fd < 0) return 1;
    char buf[128];
    ssize_t n = read_file(fd, buf, sizeof(buf)-1);
    if (n > 0) {
        buf[n] = '\0';
        printf("Read: %s\n", buf);
    }
    close_file(fd);
    return 0;
}

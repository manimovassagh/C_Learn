// Reads up to bufsize bytes from fd into buf, returns number of bytes read or -1 on error
ssize_t read_file(int fd, void *buf, size_t bufsize) {
    ssize_t bytes = read(fd, buf, bufsize);
    if (bytes < 0) {
        fprintf(stderr, "Error reading from fd %d: %s\n", fd, strerror(errno));
    }
    return bytes;
}
// Closes a file descriptor, returns 0 on success, -1 on error
int close_file(int fd) {
    if (close(fd) < 0) {
        fprintf(stderr, "Error closing file descriptor %d: %s\n", fd, strerror(errno));
        return -1;
    }
    return 0;
}
// file_utils.c - Utility functions for file operations

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Opens a file and returns its file descriptor, or -1 on error
int open_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
    }
    return fd;
}

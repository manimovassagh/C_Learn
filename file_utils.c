// Returns the size of a file in bytes, or -1 on error
off_t get_file_size(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) return -1;
    off_t size = lseek(fd, 0, SEEK_END);
    close(fd);
    return size;
}
// Copies contents from src_fd to dest_fd, returns bytes copied or -1 on error
ssize_t copy_file(int src_fd, int dest_fd) {
    char buf[1024];
    ssize_t total = 0, n;
    while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
        ssize_t written = write(dest_fd, buf, n);
        if (written != n) {
            fprintf(stderr, "Error writing during copy: %s\n", strerror(errno));
            return -1;
        }
        total += n;
    }
    if (n < 0) {
        fprintf(stderr, "Error reading during copy: %s\n", strerror(errno));
        return -1;
    }
    return total;
}
// Writes up to count bytes from buf to fd, returns number of bytes written or -1 on error
ssize_t write_file(int fd, const void *buf, size_t count) {
    ssize_t bytes = write(fd, buf, count);
    if (bytes < 0) {
        fprintf(stderr, "Error writing to fd %d: %s\n", fd, strerror(errno));
    }
    return bytes;
}
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

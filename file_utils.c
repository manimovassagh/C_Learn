// file_utils.c - Utility functions for file operations

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

// File existence
int file_exists(const char *filename) {
    return access(filename, F_OK) == 0 ? 1 : 0;
}

// File removal
int remove_file(const char *filename) {
    if (remove(filename) < 0) {
        fprintf(stderr, "Error removing file %s: %s\n", filename, strerror(errno));
        return -1;
    }
    return 0;
}

// File size
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

// Opens a file and returns its file descriptor, or -1 on error
int open_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
    }
    return fd;
}

// Creates a directory with specified permissions, returns 0 on success, -1 on error
int create_directory(const char *path, mode_t mode) {
    if (mkdir(path, mode) < 0) {
        fprintf(stderr, "Error creating directory %s: %s\n", path, strerror(errno));
        return -1;
    }
    return 0;
}

// Returns 1 if path is a directory, 0 if not, -1 on error
int is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) < 0) {
        if (errno != ENOENT) {
            fprintf(stderr, "Error checking if %s is directory: %s\n", path, strerror(errno));
            return -1;
        }
        return 0;
    }
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

// Renames a file, returns 0 on success, -1 on error
int rename_file(const char *oldname, const char *newname) {
    if (rename(oldname, newname) < 0) {
        fprintf(stderr, "Error renaming file %s to %s: %s\n", oldname, newname, strerror(errno));
        return -1;
    }
    return 0;
}

// Returns permissions of a file as a string (e.g., rwxr-xr-x), buf must be at least 10 bytes
int get_file_permissions(const char *filename, char *buf) {
    struct stat st;
    if (stat(filename, &st) < 0) {
        strcpy(buf, "??????????");
        return -1;
    }
    buf[0] = S_ISDIR(st.st_mode) ? 'd' : '-';
    buf[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    buf[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    buf[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    buf[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';
    buf[10] = '\0';
    return 0;
}

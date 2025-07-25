// file_utils.c - Utility functions for file operations

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>
#include <libgen.h>

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

// Returns the file size in kilobytes, or -1 on error
long get_file_size_kb(const char *filename) {
    off_t size = get_file_size(filename);
    if (size < 0) return -1;
    return (long)(size / 1024);
}

// Converts file size to human-readable string (buf must be at least 8 bytes)
int human_readable_size(off_t size, char *buf) {
    const char *units[] = {"B", "K", "M", "G", "T"};
    int i = 0;
    double s = (double)size;
    while (s >= 1024 && i < 4) {
        s /= 1024;
        i++;
    }
    snprintf(buf, 8, "%.1f%s", s, units[i]);
    return 0;
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

// Internal helper: set permissions string
static void set_perm_str(mode_t mode, char *buf) {
    buf[0] = S_ISDIR(mode) ? 'd' : '-';
    buf[1] = (mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (mode & S_IWUSR) ? 'w' : '-';
    buf[3] = (mode & S_IXUSR) ? 'x' : '-';
    buf[4] = (mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (mode & S_IWGRP) ? 'w' : '-';
    buf[6] = (mode & S_IXGRP) ? 'x' : '-';
    buf[7] = (mode & S_IROTH) ? 'r' : '-';
    buf[8] = (mode & S_IWOTH) ? 'w' : '-';
    buf[9] = (mode & S_IXOTH) ? 'x' : '-';
    buf[10] = '\0';
}

int get_file_permissions(const char *filename, char *buf) {
    struct stat st;
    if (stat(filename, &st) < 0) {
        strcpy(buf, "??????????");
        return -1;
    }
    set_perm_str(st.st_mode, buf);
    return 0;
}

// Returns 1 if file is readable by current user, 0 otherwise
int is_readable(const char *filename) {
    return access(filename, R_OK) == 0 ? 1 : 0;
}

// Returns 1 if file is writable by current user, 0 otherwise
int is_writable(const char *filename) {
    return access(filename, W_OK) == 0 ? 1 : 0;
}

// Returns 1 if file is executable by current user, 0 otherwise
int is_executable(const char *filename) {
    return access(filename, X_OK) == 0 ? 1 : 0;
}

// Returns last modification time of file as time_t, or -1 on error
time_t get_modification_time(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (time_t)-1;
    return st.st_mtime;
}

// Returns the user ID of the file owner, or -1 on error
uid_t get_file_owner(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (uid_t)-1;
    return st.st_uid;
}

// Returns the group ID of the file owner, or -1 on error
gid_t get_file_group(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (gid_t)-1;
    return st.st_gid;
}

// Truncates a file to the given size, returns 0 on success, -1 on error
int truncate_file(const char *filename, off_t length) {
    if (truncate(filename, length) < 0) {
        fprintf(stderr, "Error truncating file %s: %s\n", filename, strerror(errno));
        return -1;
    }
    return 0;
}

// Changes file permissions, returns 0 on success, -1 on error
int change_permissions(const char *filename, mode_t mode) {
    if (chmod(filename, mode) < 0) {
        fprintf(stderr, "Error changing permissions for %s: %s\n", filename, strerror(errno));
        return -1;
    }
    return 0;
}

// Returns the number of hard links to a file, or -1 on error
int get_link_count(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (int)st.st_nlink;
}

// Returns 1 if file is a symbolic link, 0 if not, -1 on error
int is_symlink(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) < 0) return -1;
    return S_ISLNK(st.st_mode) ? 1 : 0;
}

// Returns 1 if file is empty, 0 if not, -1 on error
int is_file_empty(const char *filename) {
    off_t size = get_file_size(filename);
    if (size < 0) return -1;
    return size == 0 ? 1 : 0;
}

// Returns the absolute path of a file, or NULL on error
char *get_absolute_path(const char *filename, char *buf, size_t buflen) {
    if (!realpath(filename, buf)) {
        fprintf(stderr, "Error getting absolute path for %s: %s\n", filename, strerror(errno));
        return NULL;
    }
    return buf;
}

// Returns 1 if file exists and is not empty, 0 otherwise
int file_exists_and_not_empty(const char *filename) {
    return file_exists(filename) && !is_file_empty(filename);
}

// Returns the basename of a file path
const char *get_basename(const char *path) {
    return basename((char *)path);
}

// file_utils.h - Header for file_utils.c

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

int open_file(const char *filename);
int close_file(int fd);
ssize_t read_file(int fd, void *buf, size_t bufsize);
ssize_t write_file(int fd, const void *buf, size_t count);
ssize_t copy_file(int src_fd, int dest_fd);
off_t get_file_size(const char *filename);
int create_directory(const char *path, mode_t mode);
int is_directory(const char *path);
int remove_file(const char *filename);
int rename_file(const char *oldname, const char *newname);
int get_file_permissions(const char *filename, char *buf);
int human_readable_size(off_t size, char *buf);
int is_readable(const char *filename);
int is_writable(const char *filename);
int is_executable(const char *filename);

#endif // FILE_UTILS_H

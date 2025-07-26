// file_utils.h - Header for file_utils.c

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <libgen.h>

int open_file(const char *filename);
int close_file(int fd);
ssize_t read_file(int fd, void *buf, size_t bufsize);
ssize_t write_file(int fd, const void *buf, size_t count);
ssize_t copy_file(int src_fd, int dest_fd);
off_t get_file_size(const char *filename);
long get_file_size_kb(const char *filename);
int create_directory(const char *path, mode_t mode);
int is_directory(const char *path);
int remove_file(const char *filename);
int rename_file(const char *oldname, const char *newname);
int get_file_permissions(const char *filename, char *buf);
int human_readable_size(off_t size, char *buf);
int is_readable(const char *filename);
int is_writable(const char *filename);
int is_executable(const char *filename);
time_t get_modification_time(const char *filename);
uid_t get_file_owner(const char *filename);
gid_t get_file_group(const char *filename);
int truncate_file(const char *filename, off_t length);
int change_permissions(const char *filename, mode_t mode);
int get_link_count(const char *filename);
int is_symlink(const char *filename);
int is_file_empty(const char *filename);
char *get_absolute_path(const char *filename, char *buf, size_t buflen);
int file_exists_and_not_empty(const char *filename);
const char *get_basename(const char *path);
const char *get_dirname(const char *path);
int is_regular_file(const char *filename);
const char *get_file_extension(const char *path);
int has_extension(const char *path, const char *ext);
int is_absolute_path(const char *path);

#endif // FILE_UTILS_H

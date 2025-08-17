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
#include <sys/statvfs.h>
#include <sys/param.h>
#include <sys/mount.h>

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

// Returns the file creation time (birth time) as time_t, or -1 on error
time_t get_creation_time(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (time_t)-1;
#ifdef __APPLE__
    return st.st_birthtime;
#else
    return (time_t)-1;
#endif
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

// Returns the directory name of a file path
const char *get_dirname(const char *path) {
    return dirname((char *)path);
}

// Returns 1 if file is a regular file, 0 if not, -1 on error
int is_regular_file(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return S_ISREG(st.st_mode) ? 1 : 0;
}

// Returns pointer to file extension in path, or NULL if none
const char *get_file_extension(const char *path) {
    const char *dot = strrchr(path, '.');
    if (!dot || dot == path) return NULL;
    return dot + 1;
}

// Returns 1 if file has the given extension, 0 otherwise
int has_extension(const char *path, const char *ext) {
    const char *file_ext = get_file_extension(path);
    if (!file_ext) return 0;
    return strcmp(file_ext, ext) == 0 ? 1 : 0;
}

// Returns 1 if path is absolute, 0 otherwise
int is_absolute_path(const char *path) {
    return path && path[0] == '/';
}

// Returns 1 if path is relative, 0 otherwise
int is_relative_path(const char *path) {
    return path && path[0] != '/';
}

// Returns 1 if file is hidden (starts with a dot), 0 otherwise
int is_hidden_file(const char *path) {
    const char *base = get_basename(path);
    return base && base[0] == '.' ? 1 : 0;
}

// Returns 1 if file is a device file (block or char), 0 if not, -1 on error
int is_device_file(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return S_ISBLK(st.st_mode) || S_ISCHR(st.st_mode) ? 1 : 0;
}

// Returns 1 if file is a FIFO (named pipe), 0 if not, -1 on error
int is_fifo(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return S_ISFIFO(st.st_mode) ? 1 : 0;
}

// Returns 1 if file is a socket, 0 if not, -1 on error
int is_socket(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return S_ISSOCK(st.st_mode) ? 1 : 0;
}

// Returns the inode number of a file, or -1 on error
ino_t get_inode(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (ino_t)-1;
    return st.st_ino;
}

// Returns the block size of a file, or -1 on error
blksize_t get_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (blksize_t)-1;
    return st.st_blksize;
}

// Returns the number of blocks allocated to a file, or -1 on error
blkcnt_t get_block_count(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (blkcnt_t)-1;
    return st.st_blocks;
}

// Returns the device ID of a file, or -1 on error
dev_t get_device_id(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (dev_t)-1;
    return st.st_dev;
}

// Returns the optimal I/O block size for file system operations, or -1 on error
long get_optimal_io_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the preferred block size for statvfs, or -1 on error
long get_statvfs_block_size(const char *filename) {
    struct statvfs vfs;
    if (statvfs(filename, &vfs) < 0) return -1;
    return (long)vfs.f_bsize;
}

// Returns the filesystem type name for a file, or NULL on error
const char *get_fs_type(const char *filename, char *buf, size_t buflen) {
    struct statfs fs;
    if (statfs(filename, &fs) < 0) return NULL;
    strncpy(buf, fs.f_fstypename, buflen - 1);
    buf[buflen - 1] = '\0';
    return buf;
}

// Returns the file access time as time_t, or -1 on error
time_t get_access_time(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (time_t)-1;
    return st.st_atime;
}

// Returns the file change time (ctime) as time_t, or -1 on error
time_t get_change_time(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return (time_t)-1;
    return st.st_ctime;
}

// Returns the file system ID for a file, or -1 on error
#include <sys/statfs.h>
fsid_t get_fs_id(const char *filename) {
    struct statfs fs;
    if (statfs(filename, &fs) < 0) return (fsid_t){-1, -1};
    return fs.f_fsid;
}

// Returns the preferred I/O alignment for a file, or -1 on error
long get_io_alignment(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's protection bits as an integer, or -1 on error
int get_protection_bits(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (int)st.st_mode & 0777;
}

// Returns the file's special mode bits (setuid, setgid, sticky), or -1 on error
int get_special_mode_bits(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (int)(st.st_mode & (S_ISUID | S_ISGID | S_ISVTX));
}

// Returns the file's raw stat structure, or -1 on error
int get_raw_stat(const char *filename, struct stat *st) {
    if (stat(filename, st) < 0) return -1;
    return 0;
}

// Returns the file's statvfs structure, or -1 on error
#include <sys/statvfs.h>
int get_raw_statvfs(const char *filename, struct statvfs *vfs) {
    if (statvfs(filename, vfs) < 0) return -1;
    return 0;
}

// Returns the file's statfs structure, or -1 on error
#include <sys/param.h>
#include <sys/mount.h>
int get_raw_statfs(const char *filename, struct statfs *fs) {
    if (statfs(filename, fs) < 0) return -1;
    return 0;
}

// Returns the file's preferred I/O size for reading, or -1 on error
long get_preferred_read_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred I/O size for writing, or -1 on error
long get_preferred_write_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred buffer size for I/O, or -1 on error
long get_preferred_buffer_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred alignment for direct I/O, or -1 on error
long get_preferred_directio_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for mmap, or -1 on error
long get_preferred_mmap_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for readahead, or -1 on error
long get_preferred_readahead_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for cache, or -1 on error
long get_preferred_cache_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for writeback, or -1 on error
long get_preferred_writeback_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for journaling, or -1 on error
long get_preferred_journal_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for metadata, or -1 on error
long get_preferred_metadata_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for snapshot, or -1 on error
long get_preferred_snapshot_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for backup, or -1 on error
long get_preferred_backup_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for mirror, or -1 on error
long get_preferred_mirror_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for compression, or -1 on error
long get_preferred_compression_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for encryption, or -1 on error
long get_preferred_encryption_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for deduplication, or -1 on error
long get_preferred_dedup_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for archiving, or -1 on error
long get_preferred_archive_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for logging, or -1 on error
long get_preferred_log_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp files, or -1 on error
long get_preferred_temp_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for swap files, or -1 on error
long get_preferred_swap_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for spool files, or -1 on error
long get_preferred_spool_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for print jobs, or -1 on error
long get_preferred_printjob_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for mail files, or -1 on error
long get_preferred_mail_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for config files, or -1 on error
long get_preferred_config_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for lock files, or -1 on error
long get_preferred_lock_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for license files, or -1 on error
long get_preferred_license_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for manifest files, or -1 on error
long get_preferred_manifest_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for resource files, or -1 on error
long get_preferred_resource_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for font files, or -1 on error
long get_preferred_font_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for image files, or -1 on error
long get_preferred_image_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for video files, or -1 on error
long get_preferred_video_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for audio files, or -1 on error
long get_preferred_audio_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for subtitle files, or -1 on error
long get_preferred_subtitle_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for playlist files, or -1 on error
long get_preferred_playlist_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for database files, or -1 on error
long get_preferred_database_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp index files, or -1 on error
long get_preferred_tempindex_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp cache index files, or -1 on error
long get_preferred_tempcacheindex_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session index files, or -1 on error
long get_preferred_tempsessionindex_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp backup index files, or -1 on error
long get_preferred_tempbackupindex_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp archive index files, or -1 on error
long get_preferred_temparchiveindex_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp archive log files, or -1 on error
long get_preferred_temparchivelog_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp resource log files, or -1 on error
long get_preferred_tempresourcelog_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session log files, or -1 on error
long get_preferred_tempsessionlog_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session log index files, or -1 on error
long get_preferred_tempsessionlogindex_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp cache files, or -1 on error
long get_preferred_tempcache_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp export files, or -1 on error
long get_preferred_tempexport_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp import files, or -1 on error
long get_preferred_tempimport_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp patch files, or -1 on error
long get_preferred_temppatch_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp snapshot files, or -1 on error
long get_preferred_tempsnapshot_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp diff files, or -1 on error
long get_preferred_tempdiff_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp merge files, or -1 on error
long get_preferred_tempmerge_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp metadata files, or -1 on error
long get_preferred_tempmetadata_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp config files, or -1 on error
long get_preferred_tempconfig_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp settings files, or -1 on error
long get_preferred_tempsettings_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp user files, or -1 on error
long get_preferred_tempuser_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp group files, or -1 on error
long get_preferred_tempgroup_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp role files, or -1 on error
long get_preferred_temprole_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp permission files, or -1 on error
long get_preferred_temppermission_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp access files, or -1 on error
long get_preferred_tempaccess_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp audit files, or -1 on error
long get_preferred_tempaudit_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp event files, or -1 on error
long get_preferred_tempevent_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp notification files, or -1 on error
long get_preferred_tempnotification_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp message files, or -1 on error
long get_preferred_tempmessage_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp mail files, or -1 on error
long get_preferred_tempmail_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp draft files, or -1 on error
long get_preferred_tempdraft_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp trash files, or -1 on error
long get_preferred_temptrash_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session backup files, or -1 on error
long get_preferred_tempsessionbackup_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session archive files, or -1 on error
long get_preferred_tempsessionarchive_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session config files, or -1 on error
long get_preferred_tempsessionconfig_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session settings files, or -1 on error
long get_preferred_tempsessionsettings_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session user files, or -1 on error
long get_preferred_tempsessionuser_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session group files, or -1 on error
long get_preferred_tempsessiongroup_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session role files, or -1 on error
long get_preferred_tempsessionrole_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session permission files, or -1 on error
long get_preferred_tempsessionpermission_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session access files, or -1 on error
long get_preferred_tempsessionaccess_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session audit files, or -1 on error
long get_preferred_tempsessionaudit_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

// Returns the file's preferred block size for temp session event files, or -1 on error
long get_preferred_tempsessionevent_block_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) < 0) return -1;
    return (long)st.st_blksize;
}

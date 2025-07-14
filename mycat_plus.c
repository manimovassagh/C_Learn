// mycat_plus.c
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 1024

void print_file(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        write(2, "Cannot open file\n", 17);
        return;
    }

    char buffer[BUF_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, BUF_SIZE)) > 0) {
        write(1, buffer, bytesRead);
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(2, "Usage: ./mycat_plus <file1> [file2...]\n", 40);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        print_file(argv[i]);
    }

    return 0;
}
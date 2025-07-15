#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// mycat_plus.c --- A version of mycat that can display line numbers

// This program reads files and prints their contents to standard output.
#define BUF_SIZE 1024

void print_file(const char *filename, int line_numbers) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        write(2, "Cannot open file\n", 17);
        return;
    }

    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    int line_number = 1;
    int new_line = 1; // flag to track line start

    while ((bytesRead = read(fd, buffer, BUF_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (new_line && line_numbers) {
                char line_buf[32];
                int len = snprintf(line_buf, sizeof(line_buf), "%6d  ", line_number++);
                write(1, line_buf, len);
                new_line = 0;
            }

            write(1, &buffer[i], 1);

            if (buffer[i] == '\n') {
                new_line = 1;
            }
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    int line_numbers = 0;
    int start_index = 1;

    if (argc < 2) {
        write(2, "Usage: ./mycat_plus [-n] <file1> [file2...]\n", 45);
        return 1;
    }

    if (strcmp(argv[1], "-n") == 0) {
        line_numbers = 1;
        start_index = 2;
    }

    for (int i = start_index; i < argc; i++) {
        print_file(argv[i], line_numbers);
    }

    return 0;
}



#include <unistd.h>
#include <fcntl.h>

// mycat_plus: Enhanced cat utility with optional line numbering
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// mycat_plus.c --- A version of mycat that can display line numbers

// This program reads files and prints their contents to standard output.
// If the -n option is provided, it will also print line numbers.
#define BUF_SIZE 1024 // Buffer size for reading file chunks

// Prints the contents of a file to stdout. If line_numbers is nonzero, prints line numbers.
void print_file(const char *filename, int line_numbers) {
    // Open the file for reading
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        // Print error if file cannot be opened
        dprintf(2, "Cannot open file: %s\n", filename);
        return;
    }

    // Variable declarations
    // Buffer for file data
    char buffer[BUF_SIZE];
    ssize_t bytesRead;
    int line_number = 1; // Current line number
    int new_line = 1; // Flag to track if at start of a new line

    // Read the file in chunks and process each byte
    while ((bytesRead = read(fd, buffer, BUF_SIZE)) > 0) {
        // Iterate through each byte in the buffer
        for (ssize_t i = 0; i < bytesRead; i++) {
            // Print line number at the start of a new line if enabled
            if (new_line && line_numbers) {
                char line_buf[32];
                int len = snprintf(line_buf, sizeof(line_buf), "%6d  ", line_number++);
                write(1, line_buf, len);
                new_line = 0;
            }

            // Write the current character to stdout
            write(1, &buffer[i], 1);

            // If newline, set flag for next line
            if (buffer[i] == '\n') {
                new_line = 1;
            }
        }
    }

    // Close the file descriptor
    close(fd);
}

// Program entry point: parses arguments and calls print_file for each file

int main(int argc, char *argv[]) {
    int line_numbers = 0; // Flag for line numbering
    int start_index = 1; // Index of first file argument

    if (argc < 2) {
        // Print usage message if not enough arguments
        write(2, "Usage: ./mycat_plus [-n] <file1> [file2...]\n", 45);
        return 1;
    }

    // Check for -n option to enable line numbering
    if (strcmp(argv[1], "-n") == 0) {
        line_numbers = 1;
        start_index = 2;
    }

    // Loop through each file argument and print its contents
    // Each file is processed in order
    for (int i = start_index; i < argc; i++) {
        print_file(argv[i], line_numbers);
    }

    // Successful execution
    // End of program
    return 0;
}



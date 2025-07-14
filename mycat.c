

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(2, "Usage: ./mycat <filename>\n", 27);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        write(2, "Error opening file\n", 19);
        return 1;
    }

    char buffer[BUF_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(fd, buffer, BUF_SIZE)) > 0) {
        write(1, buffer, bytesRead); // 1 = stdout
    }

    close(fd);
    return 0;
}
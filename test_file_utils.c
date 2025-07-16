// test_file_utils.c - Test suite for file_utils
#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test utility functions
int test_count = 0;
int test_passed = 0;

void test_result(const char *test_name, int success) {
    test_count++;
    if (success) {
        test_passed++;
        printf("✓ %s\n", test_name);
    } else {
        printf("✗ %s\n", test_name);
    }
}

// Test file_exists function
void test_file_exists() {
    // Create a temporary file
    FILE *fp = fopen("test.txt", "w");
    if (!fp) {
        test_result("file_exists", 0);
        return;
    }
    fclose(fp);

    int exists = file_exists("test.txt");
    int not_exists = file_exists("nonexistent.txt");
    
    remove("test.txt");
    test_result("file_exists", exists == 1 && not_exists == 0);
}

// Test basic read/write
void test_read_write() {
    const char *test_str = "Hello, World!\n";
    int fd = open_file("test.txt");
    if (fd < 0) {
        test_result("read_write", 0);
        return;
    }

    char buf[128];
    write_file(fd, test_str, strlen(test_str));
    close_file(fd);

    fd = open_file("test.txt");
    ssize_t n = read_file(fd, buf, sizeof(buf)-1);
    close_file(fd);
    buf[n] = '\0';
    
    remove("test.txt");
    test_result("read_write", n == strlen(test_str) && strcmp(buf, test_str) == 0);
}

int main() {
    printf("Running file_utils tests...\n\n");
    
    test_file_exists();
    test_read_write();
    
    printf("\nTests completed: %d passed, %d failed\n", 
           test_passed, test_count - test_passed);
    return test_passed == test_count ? 0 : 1;
}

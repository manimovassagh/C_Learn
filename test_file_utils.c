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

// Test directory creation
void test_create_directory() {
    const char *test_dir = "test_dir";
    int result = create_directory(test_dir, 0755);
    int exists = file_exists(test_dir);
    
    // Clean up
    rmdir(test_dir);
    
    test_result("create_directory", result == 0 && exists == 1);
}

// Test directory checking
void test_is_directory() {
    // Create a test directory
    const char *test_dir = "test_dir";
    create_directory(test_dir, 0755);
    
    // Create a test file
    const char *test_file = "test.txt";
    FILE *fp = fopen(test_file, "w");
    if (fp) fclose(fp);
    
    int dir_result = is_directory(test_dir);
    int file_result = is_directory(test_file);
    int nonexist_result = is_directory("nonexistent");
    
    // Clean up
    rmdir(test_dir);
    remove(test_file);
    
    test_result("is_directory", 
                dir_result == 1 && 
                file_result == 0 && 
                nonexist_result == 0);
}

// Test file removal
void test_remove_file() {
    const char *test_file = "test_remove.txt";
    FILE *fp = fopen(test_file, "w");
    if (fp) fclose(fp);
    int removed = remove_file(test_file);
    int exists = file_exists(test_file);
    test_result("remove_file", removed == 0 && exists == 0);
}

// Test file renaming
void test_rename_file() {
    const char *oldname = "test_rename.txt";
    const char *newname = "test_renamed.txt";
    FILE *fp = fopen(oldname, "w");
    if (fp) fclose(fp);
    int renamed = rename_file(oldname, newname);
    int exists_old = file_exists(oldname);
    int exists_new = file_exists(newname);
    remove(newname);
    test_result("rename_file", renamed == 0 && exists_old == 0 && exists_new == 1);
}

int main() {
    printf("Running file_utils tests...\n\n");
    
    test_file_exists();
    test_read_write();
    test_create_directory();
    test_is_directory();
    test_remove_file();
    test_rename_file();
    
    printf("\nTests completed: %d passed, %d failed\n", 
           test_passed, test_count - test_passed);
    return test_passed == test_count ? 0 : 1;
}

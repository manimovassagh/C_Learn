// test_path_utils.c - Simple tests for path_utils
#include "path_utils.h"
#include <stdio.h>

int main() {
    const char *path = "/usr/local/bin/example.txt";
    printf("is_absolute_path: %d\n", is_absolute_path(path));
    printf("is_relative_path: %d\n", is_relative_path(path));
    printf("basename: %s\n", get_basename(path));
    printf("dirname: %s\n", get_dirname(path));
    return 0;
}

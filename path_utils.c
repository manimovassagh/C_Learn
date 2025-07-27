#include "path_utils.h"
#include <string.h>
#include <libgen.h>

// Path utility function implementations will go here

int is_absolute_path(const char *path) {
    return path && path[0] == '/';
}

int is_relative_path(const char *path) {
    return path && path[0] != '/';
}

const char *get_basename(const char *path) {
    return basename((char *)path);
}

const char *get_dirname(const char *path) {
    return dirname((char *)path);
}

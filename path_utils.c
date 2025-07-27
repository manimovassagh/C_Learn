#include "path_utils.h"
#include <string.h>

// Path utility function implementations will go here

int is_absolute_path(const char *path) {
    return path && path[0] == '/';
}

#ifndef PATH_UTILS_H
#define PATH_UTILS_H

// Path utility function prototypes will go here

int is_absolute_path(const char *path);
int is_relative_path(const char *path);
const char *get_basename(const char *path);

#endif // PATH_UTILS_H

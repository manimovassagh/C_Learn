#ifndef ENV_UTILS_H
#define ENV_UTILS_H

// Environment utility function prototypes will go here

const char *get_env(const char *name);
int set_env(const char *name, const char *value, int overwrite);
int unset_env(const char *name);
void print_env(const char *name);

#endif // ENV_UTILS_H

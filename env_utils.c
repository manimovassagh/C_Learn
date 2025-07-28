#include "env_utils.h"
#include <stdlib.h>
#include <stdio.h>

// Environment utility function implementations will go here

const char *get_env(const char *name) {
    return getenv(name);
}

int set_env(const char *name, const char *value, int overwrite) {
    return setenv(name, value, overwrite);
}

int unset_env(const char *name) {
    return unsetenv(name);
}

void print_env(const char *name) {
    const char *val = getenv(name);
    if (val) {
        printf("%s=%s\n", name, val);
    } else {
        printf("%s is not set\n", name);
    }
}

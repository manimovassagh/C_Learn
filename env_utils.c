#include "env_utils.h"
#include <stdlib.h>

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

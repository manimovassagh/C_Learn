#include "env_utils.h"
#include <stdlib.h>

// Environment utility function implementations will go here

const char *get_env(const char *name) {
    return getenv(name);
}

#include "string_utils.h"

// Returns the length of a string
int str_length(const char *s) {
    int len = 0;
    while (s && *s++) len++;
    return len;
}

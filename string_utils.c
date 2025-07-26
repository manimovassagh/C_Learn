#include "string_utils.h"

// Returns the length of a string
int str_length(const char *s) {
    int len = 0;
    while (s && *s++) len++;
    return len;
}

// Copies src to dest, returns dest
char *str_copy(char *dest, const char *src) {
    char *d = dest;
    while ((*d++ = *src++));
    return dest;
}

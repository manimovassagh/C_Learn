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

// Compares two strings, returns 0 if equal, <0 if s1<s2, >0 if s1>s2
int str_compare(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// Concatenates src to dest, returns dest
char *str_concat(char *dest, const char *src) {
    char *d = dest;
    while (*d) d++;
    while ((*d++ = *src++));
    return dest;
}

// Finds first occurrence of c in s, returns pointer or NULL
char *str_find(const char *s, char c) {
    while (*s) {
        if (*s == c) return (char *)s;
        s++;
    }
    return NULL;
}

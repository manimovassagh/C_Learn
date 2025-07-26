#include "string_utils.h"
#include <ctype.h>
#include <stdlib.h>

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

// Finds last occurrence of c in s, returns pointer or NULL
char *str_rfind(const char *s, char c) {
    const char *last = NULL;
    while (*s) {
        if (*s == c) last = s;
        s++;
    }
    return (char *)last;
}

// Converts string to uppercase in place
void str_to_upper(char *s) {
    while (s && *s) {
        *s = toupper((unsigned char)*s);
        s++;
    }
}

// Converts string to lowercase in place
void str_to_lower(char *s) {
    while (s && *s) {
        *s = tolower((unsigned char)*s);
        s++;
    }
}

// Returns 1 if s starts with prefix, 0 otherwise
int str_starts_with(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s++ != *prefix++) return 0;
    }
    return 1;
}

// Returns 1 if s ends with suffix, 0 otherwise
int str_ends_with(const char *s, const char *suffix) {
    int slen = str_length(s);
    int suflen = str_length(suffix);
    if (suflen > slen) return 0;
    return str_compare(s + slen - suflen, suffix) == 0;
}

// Reverses a string in place
void str_reverse(char *s) {
    int len = str_length(s);
    for (int i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = tmp;
    }
}

// Removes leading and trailing whitespace from s, returns s
char *str_strip(char *s) {
    char *end;
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    end = s + str_length(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return s;
}

// Counts occurrences of c in s
int str_count_char(const char *s, char c) {
    int count = 0;
    while (*s) {
        if (*s == c) count++;
        s++;
    }
    return count;
}

// Returns a malloc'd copy of s, or NULL on error
char *str_dup(const char *s) {
    int len = str_length(s);
    char *copy = malloc(len + 1);
    if (!copy) return NULL;
    str_copy(copy, s);
    return copy;
}

// Returns 1 if s is NULL or empty, 0 otherwise
int str_is_empty(const char *s) {
    return !s || *s == '\0';
}

// Removes trailing newline from s, if present
void str_chomp(char *s) {
    int len = str_length(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

// Pads s on the left with pad_char to total_len, returns s
char *str_pad_left(char *s, int total_len, char pad_char) {
    int len = str_length(s);
    if (len >= total_len) return s;
    int pad = total_len - len;
    for (int i = len; i >= 0; i--) s[i + pad] = s[i];
    for (int i = 0; i < pad; i++) s[i] = pad_char;
    return s;
}

// Pads s on the right with pad_char to total_len, returns s
char *str_pad_right(char *s, int total_len, char pad_char) {
    int len = str_length(s);
    if (len >= total_len) return s;
    for (int i = len; i < total_len; i++) s[i] = pad_char;
    s[total_len] = '\0';
    return s;
}

// Fills s with c repeated n times, null-terminated, returns s
char *str_repeat(char *s, char c, int n) {
    for (int i = 0; i < n; i++) s[i] = c;
    s[n] = '\0';
    return s;
}

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

// Returns the length of a string
int str_length(const char *s);

// Copies string from source to destination
char *str_copy(char *dest, const char *src);

// Compares two strings
int str_compare(const char *s1, const char *s2);

// Concatenates two strings
char *str_concat(char *dest, const char *src);

// Finds the first occurrence of character c in string s
char *str_find(const char *s, char c);

#endif // STRING_UTILS_H

#include "char_utils.h"
#include <ctype.h>

// Character utility function implementations will go here

int is_alpha(char c) {
    return isalpha((unsigned char)c);
}

int is_digit(char c) {
    return isdigit((unsigned char)c);
}

int is_alnum(char c) {
    return isalnum((unsigned char)c);
}

int is_space(char c) {
    return isspace((unsigned char)c);
}

int to_upper(char c) {
    return toupper((unsigned char)c);
}

int to_lower(char c) {
    return tolower((unsigned char)c);
}

#include "char_utils.h"
#include <ctype.h>

// Character utility function implementations will go here

int is_alpha(char c) {
    return isalpha((unsigned char)c);
}

int is_digit(char c) {
    return isdigit((unsigned char)c);
}

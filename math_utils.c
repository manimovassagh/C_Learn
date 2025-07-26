#include "math_utils.h"

// Math utility function implementations will go here

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

double divide(int a, int b) {
    if (b == 0) return 0.0;
    return (double)a / b;
}

int mod(int a, int b) {
    if (b == 0) return 0;
    return a % b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int abs_val(int a) {
    return a < 0 ? -a : a;
}

int pow_int(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

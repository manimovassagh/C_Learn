// test_float_utils.c - Simple tests for float_utils
#include "float_utils.h"
#include <stdio.h>

int main() {
    double a = -3.7, b = 2.5;
    printf("abs: %.2f\n", float_abs(a));
    printf("min: %.2f\n", float_min(a, b));
    printf("max: %.2f\n", float_max(a, b));
    printf("round: %.2f\n", float_round(a));
    printf("floor: %.2f\n", float_floor(a));
    printf("ceil: %.2f\n", float_ceil(a));
    printf("pow: %.2f\n", float_pow(2.0, 3.0));
    return 0;
}

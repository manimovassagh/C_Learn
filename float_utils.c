#include "float_utils.h"
#include <math.h>

// Float utility function implementations will go here

double float_abs(double x) {
    return x < 0 ? -x : x;
}

double float_min(double a, double b) {
    return a < b ? a : b;
}

double float_max(double a, double b) {
    return a > b ? a : b;
}

double float_round(double x) {
    return round(x);
}

double float_floor(double x) {
    return floor(x);
}

double float_ceil(double x) {
    return ceil(x);
}

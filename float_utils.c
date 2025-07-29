#include "float_utils.h"

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

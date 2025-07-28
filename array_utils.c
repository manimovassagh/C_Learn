#include "array_utils.h"

// Array utility function implementations will go here

int array_sum(const int *arr, int len) {
    int sum = 0;
    for (int i = 0; i < len; ++i) {
        sum += arr[i];
    }
    return sum;
}

double array_average(const int *arr, int len) {
    if (len == 0) return 0.0;
    return (double)array_sum(arr, len) / len;
}

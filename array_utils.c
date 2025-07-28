#include "array_utils.h"

// Array utility function implementations will go here

int array_sum(const int *arr, int len) {
    int sum = 0;
    for (int i = 0; i < len; ++i) {
        sum += arr[i];
    }
    return sum;
}

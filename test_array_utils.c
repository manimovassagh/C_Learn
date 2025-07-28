// test_array_utils.c - Simple tests for array_utils
#include "array_utils.h"
#include <stdio.h>

void print_array(const int *arr, int len) {
    for (int i = 0; i < len; ++i) printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int len = 5;
    printf("sum: %d\n", array_sum(arr, len));
    printf("avg: %.2f\n", array_average(arr, len));
    printf("min: %d\n", array_min(arr, len));
    printf("max: %d\n", array_max(arr, len));
    array_reverse(arr, len);
    print_array(arr, len);
    return 0;
}

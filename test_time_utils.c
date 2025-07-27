// test_time_utils.c - Simple tests for time_utils
#include "time_utils.h"
#include <stdio.h>

int main() {
    printf("Current time: %ld\n", get_current_time());
    char buf[32];
    printf("Formatted: %s\n", format_time(get_current_time(), buf, sizeof(buf)));
    printf("Leap year 2024: %d\n", is_leap_year(2024));
    printf("Days in Feb 2024: %d\n", days_in_month(2024, 2));
    printf("Current year: %d\n", get_current_year());
    printf("Current month: %d\n", get_current_month());
    printf("Current day: %d\n", get_current_day());
    printf("Current weekday: %d\n", get_current_weekday());
    printf("Unix timestamp: %ld\n", get_unix_timestamp());
    return 0;
}

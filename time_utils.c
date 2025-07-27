#include "time_utils.h"
#include <time.h>
#include <stdio.h>

// Time utility function implementations will go here

time_t get_current_time() {
    return time(NULL);
}

char *format_time(time_t t, char *buf, size_t buflen) {
    struct tm *tm_info = localtime(&t);
    strftime(buf, buflen, "%Y-%m-%d %H:%M:%S", tm_info);
    return buf;
}

int is_leap_year(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int days_in_month(int year, int month) {
    static const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && is_leap_year(year)) return 29;
    return days[month - 1];
}

int get_current_year() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    return tm_info->tm_year + 1900;
}

int get_current_month() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    return tm_info->tm_mon + 1;
}

int get_current_day() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    return tm_info->tm_mday;
}

int get_current_weekday() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    return tm_info->tm_wday;
}

long get_unix_timestamp() {
    return (long)time(NULL);
}

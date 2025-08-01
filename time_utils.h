#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <ctime> // Include ctime for time_t definition

// Time utility function prototypes will go here

time_t get_current_time();
char *format_time(time_t t, char *buf, size_t buflen);
int is_leap_year(int year);
int days_in_month(int year, int month);
int get_current_year();
int get_current_month();
int get_current_day();
int get_current_weekday();
long get_unix_timestamp();

#endif // TIME_UTILS_H

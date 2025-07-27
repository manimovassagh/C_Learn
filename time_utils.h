#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <ctime> // Include ctime for time_t definition

// Time utility function prototypes will go here

time_t get_current_time();
char *format_time(time_t t, char *buf, size_t buflen);

#endif // TIME_UTILS_H

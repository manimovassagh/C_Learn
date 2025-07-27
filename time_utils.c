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

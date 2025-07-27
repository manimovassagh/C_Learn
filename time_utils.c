#include "time_utils.h"
#include <time.h>

// Time utility function implementations will go here

time_t get_current_time() {
    return time(NULL);
}

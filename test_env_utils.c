// test_env_utils.c - Simple tests for env_utils
#include "env_utils.h"
#include <stdio.h>

int main() {
    set_env("TEST_ENV", "hello", 1);
    print_env("TEST_ENV");
    unset_env("TEST_ENV");
    print_env("TEST_ENV");
    return 0;
}

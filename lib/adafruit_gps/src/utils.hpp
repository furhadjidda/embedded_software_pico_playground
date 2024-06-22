#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <ctype.h>
#include <stdlib.h>

#include "pico/stdlib.h"

typedef uint8_t byte;

#define RAD_TO_DEG 57.29578

static uint64_t millis() { return time_us_64() / 1000; }

/*!
    @brief Checks whether a string starts with a specified prefix
    @param str Pointer to a string
    @param prefix Pointer to the prefix
    @return True if str starts with prefix, false otherwise
*/
static bool StrStartsWith(const char *str, const char *prefix) {
    while (*prefix) {
        if (*prefix++ != *str++) return false;
    }
    return true;
}

// Define the min template function
template <typename T>
T min(T a, T b) {
    return (a < b) ? a : b;
}

// Define the max template function
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

#endif
#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <stdlib.h>
#include "pico/stdlib.h"
#include <ctype.h>

typedef uint8_t byte;

#define RAD_TO_DEG 57.29578

static uint64_t millis() {
    return time_us_64() / 1000;
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
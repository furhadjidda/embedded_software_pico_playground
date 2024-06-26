
/*
 *   This file is part of embedded software pico playground project.
 *
 *   embedded software pico playground projec is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   embedded software pico playground project is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License v3.0
 *   along with embedded software pico playground project.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
// Pico SDK
#ifndef BUILD_FOR_HOST
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#endif

using std::string;
using std::vector;

/*
 * PROTOTYPES
 */
namespace Utils {
vector<string> split_to_lines(string str, string sep = "\r\n");
string split_msg(string msg, uint32_t want_line);
string get_sms_number(string line);
string get_field_value(string line, uint32_t field_number);
string uppercase(string base);
uint32_t bcd(uint32_t base);
void log_device_info(void);
void log_debug(const string msg);
}  // namespace Utils

#endif  // UTILS_HEADER

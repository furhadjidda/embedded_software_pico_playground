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
#include <gtest/gtest.h>

#include "utils.h"
// Test the add function
TEST(UtilsTests, TestingSplit) {
    string test = "testing1\r\ntesting2";
    vector<string> expected = {"testing1", "testing2"};
    EXPECT_EQ(expected, Utils::split_to_lines(test));
}
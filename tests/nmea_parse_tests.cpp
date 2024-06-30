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

#include "Adafruit_GPS.hpp"
#include "i2c_wrapper.hpp"
// Test fixture for Adafruit_GPS
class AdafruitGPSTest : public ::testing::Test {
   protected:
    i2c_inst_t mock_i2c;
    Adafruit_GPS gps;

    // Test fixture constructor
    AdafruitGPSTest() : gps(&mock_i2c) {}  // Initialize Adafruit_GPS with mock arguments

    void SetUp() override {
        // Initialize any necessary objects or states here
    }

    void TearDown() override {
        // Clean up any objects or states here
    }
};

TEST_F(AdafruitGPSTest, ParseValidGGA) {
    char nmea[] = "$GNGGA,033954.000,3850.2364,N,09447.3419,W,1,4,2.08,318.4,M,-30.0,M,,*7A";
    ASSERT_TRUE(gps.Parse(nmea));
}

TEST_F(AdafruitGPSTest, ParseInvalidChecksum) {
    char nmea[] = "$GPGGA,123456.00,3723.2475,N,12158.3416,W,1,08,0.9,545.4,M,46.9,M,,*00";
    ASSERT_FALSE(gps.Parse(nmea));
}

// Add more tests as needed

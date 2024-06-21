// Test code for Adafruit GPS That Support Using I2C
//
// This code shows how to parse data from the I2C GPS
//
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada

#include <stdio.h>

#include <Adafruit_GPS.hpp>
//#include "utils.hpp"
#include <string.h>
// Connect to the GPS on the hardware I2C port
Adafruit_GPS GPS(i2c0);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();

void setup() {
    GPS.Init(0x10);  // The I2C address to use is 0x10
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix
    // data) including altitude
    GPS.SendCommand(reinterpret_cast<const uint8_t*>(PMTK_SET_NMEA_OUTPUT_RMCGGA), strlen(PMTK_SET_NMEA_OUTPUT_RMCGGA));
    // uncomment this line to turn on only the "minimum recommended" data
    // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or
    // RMC+GGA since the parser doesn't care about other sentences at this time
    // Set the update rate
    GPS.SendCommand(reinterpret_cast<const uint8_t*>(PMTK_SET_NMEA_UPDATE_1HZ),
                    strlen(PMTK_SET_NMEA_UPDATE_1HZ));  // 1 Hz update rate
    // For the parsing code to work nicely and have time to sort thru the data,
    // and print it out we don't suggest using anything higher than 1 Hz

    // Request updates on antenna status, comment out to keep quiet
    GPS.SendCommand(reinterpret_cast<const uint8_t*>(PGCMD_ANTENNA), strlen(PGCMD_ANTENNA));

    sleep_ms(1000);

    // Ask for firmware version
    printf("%d", PMTK_Q_RELEASE);
}

void loop()  // run over and over again
{
    // read data from the GPS in the 'main loop'
    char c = GPS.ReadData();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO) {
        if (c) {
            printf("%c", c);
        }
    }
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.NewNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trying to print out data
        printf("%s\n", GPS.LastNMEA());    // this also sets the newNMEAreceived()
                                           // flag to false
        if (!GPS.Parse(GPS.LastNMEA())) {  // this also sets the
                                           // newNMEAreceived() flag to false
            return;                        // we can fail to parse a sentence in which case we should
                                           // just wait for another
        }
    }

    // approximately every 2 seconds or so, print out the current stats
    if (millis() - timer > 2000) {
        timer = millis();  // reset the timer
        printf("\nTime: %02d:%02d:%02d.%03d\n", GPS.hour, GPS.minute, GPS.seconds, GPS.milliseconds);
        printf("Date: %02d/%02d/20%d\n", GPS.day, GPS.month, GPS.year);
        printf("Fix: %d quality: %d\n", (int)GPS.fix, (int)GPS.fixquality);
        if (GPS.fix) {
            printf("Location: %.4f %c, %.4f %c\n", GPS.latitude, GPS.lat, GPS.longitude, GPS.lon);
            printf("Speed (knots): %f\n", GPS.speed);
            printf("Angle: %f\n", GPS.angle);
            printf("Altitude: %f\n", GPS.altitude);
            printf("Satellites: %d\n", (int)GPS.satellites);
        }
    }
}

int main() {
    stdio_init_all();
    sleep_ms(1000);
    setup();

    while (1) {
        // printf("testing\n");
        loop();
    }

    return 0;
}
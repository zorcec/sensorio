/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <Arduino.h>

#include <logger.h>
#include <connectivity.h>
#include <configurations.h>
#include <notifications.h>
#include <sensors/sensorCDS1.h>

bool SensorCDS1::isActive = false;

void SensorCDS1::initialize() {
    Logger::info("-> initializing CDS1");
    SensorCDS1::isActive = true;
    SensorCDS1::refresh();
};

void SensorCDS1::loop() {

};

bool SensorCDS1::refresh() {
    if (SensorCDS1::isActive) {
        if (!Notifications::getCurrentPowerState()) {
            int16_t readings = analogRead(PIN_A0);
            Sensors::data.analogValue = readings / 4;                         //                          dark |   0 - 255 | bright
            Sensors::data.brightness = 1 - (float)readings / (float)1024;     // calculates percentages   dark | 100 - 0   | bright
            return true;
        } else {
            Logger::trace("CDS1 will not refresh because LED is on");
            return false;
        }
    }
    return false;
}

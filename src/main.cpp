/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>
#include <sensors.h>
#include <notifications.h>
#include <administration.h>
#include <permanentStorage.h>
#include <airQuality.h>
#include <passiveTracking.h>

void setup() {
    delay(500);
    Configurations::initialize();
    Logger::initialize();
    // PassiveTracking::initialize();
    switch (Configurations::data.DEVICE_TYPE) {
        case DeviceType::BOX:
        case DeviceType::WALL_MOUNT:
            Connectivity::initialize();
            PermanentStorage::initialize();
            Sensors::initialize();
            Administration::initialize();
            AirQuality::initialize();
            Notifications::initialize();
            break;
        case DeviceType::FINDER:
            Administration::initialize();
            break;
    }
    
}

void loop() {
    Connectivity::loop();
    Sensors::loop();
    Notifications::loop();
    Administration::loop();
    AirQuality::loop();
    PassiveTracking::loop();
}

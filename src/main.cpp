#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>
#include <sensors.h>
#include <notifications.h>
#include <administration.h>
#include <permanentStorage.h>
#include <airQuality.h>

void setup() {
    Configurations::initialize();
    Logger::initialize();
    PermanentStorage::initialize();
    Administration::initialize();
    Sensors::initialize();
    AirQuality::initialize();
    Connectivity::initialize();
    Notifications::initialize();
}

void loop() {
    Connectivity::loop();
    Sensors::loop();
    Notifications::loop();
    Administration::loop();
    AirQuality::loop();
}

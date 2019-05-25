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
    delay(1);
    Configurations::initialize();
    Logger::initialize();
    Connectivity::initialize();
    PermanentStorage::initialize();
    Sensors::initialize();
    Administration::initialize();
    AirQuality::initialize();
    Notifications::initialize();
}

void loop() {
    Connectivity::loop();
    Sensors::loop();
    Notifications::loop();
    Administration::loop();
    AirQuality::loop();
}

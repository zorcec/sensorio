#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>
#include <sensors.h>
#include <notifications.h>

void setup() {
    Logger::initialize();
    Sensors::initialize();
    Connectivity::initialize();
    Notifications::initialize();
}

void loop() {
    Connectivity::loop();
    Sensors::loop();
    Notifications::loop();
}

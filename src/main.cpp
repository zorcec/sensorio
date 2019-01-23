#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>
#include <sensors.h>
#include <notifications.h>
#include <administration.h>

void setup() {
    Configurations::initialize();
    Logger::initialize();
    Administration::initialize();
    Sensors::initialize();
    Connectivity::initialize();
    Notifications::initialize();
}

void loop() {
    Connectivity::loop();
    Sensors::loop();
    Notifications::loop();
    Administration::loop();
}

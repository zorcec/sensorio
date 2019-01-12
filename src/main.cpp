#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>
#include <sensors.h>

void setup() {
    Logger::initialize();
    Connectivity::initialize();
    Sensors::initialize();
}

void loop() {
    Connectivity::loop();
    Sensors::loop();
}

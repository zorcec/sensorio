#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>
#include <sensors.h>

void setup() {
    Logger::initialize();
    Sensors::initialize();
}

void loop() {
    delay(Configurations::SENSORS_REFRESH_RATE);
    Sensors::loop();
    Connectivity::loop();
}

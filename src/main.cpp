#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>
#include <sensors.h>
#include <notifications.h>

void setup() {
    Logger::initialize();
    Notifications::initialize();
    Sensors::initialize();
    Connectivity::initialize();
}

void loop() {
    Connectivity::loop();
    Sensors::loop();

    delay(Configurations::SLEEP);
}

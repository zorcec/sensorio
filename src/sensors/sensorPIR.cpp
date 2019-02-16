#include <Arduino.h>

#include <logger.h>
#include <connectivity.h>
#include <configurations.h>
#include <sensors/sensorPIR.h>

void SensorPIR::initialize() {
    Logger::info("Initializing PIR");
    Logger::debug("-> on GPIO" + String(Configurations::data.SENSORS_PIR_GPIO));

    pinMode(Configurations::data.SENSORS_PIR_GPIO, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Configurations::data.SENSORS_PIR_GPIO), SensorPIR::interupt, CHANGE);
};

void SensorPIR::loop() {

};

void SensorPIR::interupt() {
    int input = digitalRead(Configurations::data.SENSORS_PIR_GPIO);
    Connectivity::sendEvent("MOTION", String(input));
};

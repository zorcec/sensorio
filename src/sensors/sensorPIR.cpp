#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorPIR.h>

void SensorPIR::initialize() {
    Logger::info("Initializing PIR");
    Logger::debug("-> on GPIO" + String(Configurations::SENSORS_PIR_GPIO));

    pinMode(Configurations::SENSORS_PIR_GPIO, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Configurations::SENSORS_PIR_GPIO), SensorPIR::interupt, HIGH);
};

void SensorPIR::loop() {

};

void SensorPIR::interupt() {
    int value = digitalRead(Configurations::SENSORS_PIR_GPIO);
    Logger::log("PIR motion detected");
    Logger::debug("-> value: " + String(value));
};

/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <Arduino.h>

#include <logger.h>
#include <connectivity.h>
#include <configurations.h>
#include <sensors/sensorPIR.h>

bool SensorPIR::isActive = false;

void SensorPIR::initialize() {
    Logger::info("Initializing PIR");
    if (Configurations::data.SENSORS_PIR_GPIO > 0) {
        Logger::debug("-> on GPIO" + String(Configurations::data.SENSORS_PIR_GPIO));
        pinMode(Configurations::data.SENSORS_PIR_GPIO, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(Configurations::data.SENSORS_PIR_GPIO), SensorPIR::interupt, CHANGE);
        SensorPIR::isActive = true;
    } else {
        Logger::debug("-> disabled");
    }
};

void SensorPIR::loop() {

};

void ICACHE_RAM_ATTR SensorPIR::interupt() {
    int input = digitalRead(Configurations::data.SENSORS_PIR_GPIO);
    Connectivity::sendEvent("MOTION", String(input));
};

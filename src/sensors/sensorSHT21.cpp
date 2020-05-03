/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorSHT21.h>

#include "SHT2x.h";

bool SensorSHT21::isActive = false;

void SensorSHT21::initialize() {
    SensorSHT21::isActive = true;
    Logger::info("Initializing SHT21");
    SensorSHT21::refresh();
};

bool SensorSHT21::refresh() {
    if (SensorSHT21::isActive) {
        Logger::trace("Refreshing SHT21");
        Sensors::data.temperature = SHT2x.GetTemperature();
        Sensors::data.humidity = SHT2x.GetHumidity();
        Logger::trace("-> temperature:\t" + String(Sensors::data.temperature));
        Logger::trace("-> humidity:\t" + String(Sensors::data.humidity));
        return true;
    }
    return false;
};

void SensorSHT21::loop() {

}

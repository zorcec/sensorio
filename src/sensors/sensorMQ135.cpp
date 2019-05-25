#include <Arduino.h>

#include <logger.h>
#include <connectivity.h>
#include <configurations.h>
#include <sensors/sensorMQ135.h>

bool SensorMQ135::isActive = false;

void SensorMQ135::initialize() {
    Logger::info("-> initializing MQ135");
    SensorMQ135::isActive = true;
    SensorMQ135::refresh();
};

void SensorMQ135::loop() {

};

bool SensorMQ135::refresh() {
    if (SensorMQ135::isActive) {
        int16_t readings = analogRead(PIN_A0);
        Sensors::data.analogValue = readings / 4 + Configurations::data.POLUTION_VALUE_OFFSET;    //                          clean |   0 - 255 | poluted
        Sensors::data.airQuality = 1 - (float)readings / (float)1024;                             // calculates percentages   clean | 100 - 0   | poluted
        return true;
    }
    return false;
}

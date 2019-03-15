#include <Arduino.h>

#include <logger.h>
#include <connectivity.h>
#include <configurations.h>
#include <sensors/sensorMQ135.h>

Timer<1> SensorMQ135::sensorReadTimer;

void SensorMQ135::initialize() {
    Logger::info("Initializing MQ135");
    SensorMQ135::refresh();
    SensorMQ135::sensorReadTimer.every(
        Configurations::data.REFRESH_INTERVALS.MQ135, 
        [](void*) -> bool { return SensorMQ135::refresh(); }
    );
};

void SensorMQ135::loop() {
    SensorMQ135::sensorReadTimer.tick();
};

bool SensorMQ135::refresh() {
    int16_t readings = analogRead(PIN_A0);
    Sensors::data.polutionValue = readings / 4 + Configurations::data.POLUTION_VALUE_OFFSET;  //                          clean |   0 - 255 | poluted
    Sensors::data.airQuality = 1 - (float)readings / (float)1024;                             // calculates percentages   clean | 100 - 0   | poluted
    return true;
}

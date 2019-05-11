#include <Arduino.h>

#include <logger.h>
#include <connectivity.h>
#include <configurations.h>
#include <sensors/sensorCDS1.h>

Timer<1> SensorCDS1::sensorReadTimer;

void SensorCDS1::initialize() {
    Logger::info("Initializing CDS1");
    SensorCDS1::refresh();
    SensorCDS1::sensorReadTimer.every(
        Configurations::data.REFRESH_INTERVALS.ANALOG, 
        [](void*) -> bool { return SensorCDS1::refresh(); }
    );
};

void SensorCDS1::loop() {
    SensorCDS1::sensorReadTimer.tick();
};

bool SensorCDS1::refresh() {
    int16_t readings = analogRead(PIN_A0);
    Sensors::data.analogValue = readings / 4;                         //                          dark |   0 - 255 | bright
    Sensors::data.brightness = 1 - (float)readings / (float)1024;     // calculates percentages   dark | 100 - 0   | bright
    return true;
}

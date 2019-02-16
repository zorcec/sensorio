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
    Sensors::data.air = analogRead(11);
    return true;
}

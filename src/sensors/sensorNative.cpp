#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorNative.h>

Timer<1> SensorNative::sensorReadTimer;

void SensorNative::initialize() {
    Logger::info("Initializing NATIVE");
    SensorNative::refresh();
    SensorNative::sensorReadTimer.every(
        Configurations::data.REFRESH_INTERVALS.BME280, 
        [](void*) -> bool { return SensorNative::refresh(); }
    );
};

void SensorNative::loop() {
    SensorNative::sensorReadTimer.tick();
};

bool SensorNative::refresh() {
    Logger::trace("Refreshing NATIVE");
    Sensors::data.RSSI = WiFi.RSSI(); 
    Logger::trace("-> RSSI:\t" + String(Sensors::data.RSSI));
    return true;
};

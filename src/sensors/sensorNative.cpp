#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorNative.h>

Timer<1> SensorNative::sensorReadTimer;

void SensorNative::initialize() {
    Logger::info("Initializing NATIVE");
    SensorNative::sensorReadTimer.every(Configurations::REFRESH_INTERVALS.native, SensorNative::refresh);
};

void SensorNative::loop() {
    SensorNative::sensorReadTimer.tick();
};

bool SensorNative::refresh(void *) {
    Logger::trace("Refreshing NATIVE");
    Sensors::data.RSSI = WiFi.RSSI(); 
    Logger::trace("-> RSSI:\t" + String(Sensors::data.RSSI));
    return true;
};

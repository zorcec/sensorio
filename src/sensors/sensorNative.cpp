#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorNative.h>

bool SensorNative::isActive = false;

void SensorNative::initialize() {
    Logger::info("Initializing NATIVE");
    SensorNative::isActive = true;
    SensorNative::refresh();
};

void SensorNative::loop() {

};

bool SensorNative::refresh() {
    if (SensorNative::isActive) {
        Logger::trace("Refreshing NATIVE");
        Sensors::data.RSSI = WiFi.RSSI(); 
        Logger::trace("-> RSSI:\t" + String(Sensors::data.RSSI));
        return true;
    }
    return false;
};

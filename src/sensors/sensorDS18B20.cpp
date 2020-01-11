/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <Arduino.h>

#include <configurations.h>
#include <logger.h>
#include <sensors/sensorDS18B20.h>

bool SensorDS18B20::isActive = false;
OneWire SensorDS18B20::oneWire;
DallasTemperature SensorDS18B20::sensor;

void SensorDS18B20::initialize() {
    Logger::info("-> initializing DS18B20");
    SensorDS18B20::oneWire.begin(Configurations::data.DS18B20_GPIO);
    SensorDS18B20::sensor.setOneWire(&SensorDS18B20::oneWire);
    SensorDS18B20::sensor.begin();
    SensorDS18B20::isActive = true;
    SensorDS18B20::refresh();
};

void SensorDS18B20::loop() {

};

bool SensorDS18B20::refresh() {
    if (SensorDS18B20::isActive) {
       SensorDS18B20::sensor.requestTemperatures();
       Sensors::data.temperature = SensorDS18B20::sensor.getTempCByIndex(0);
    }
    return false;
}

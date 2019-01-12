#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorBME280.h>

#include "BME280I2C.h"

BME280I2C sensor;
BME280::TempUnit temperatureUnit(BME280::TempUnit_Celsius);
BME280::PresUnit pressureUnit(BME280::PresUnit_Pa);

void SensorBME280::initialize() {
    Logger::info("Initializing BME280");
    if (!sensor.begin()) {
        Logger::warn("BMC280 is not responding");
    }
    switch(sensor.chipModel()) {
        case BME280::ChipModel_BME280:
            Logger::info("-> Sensor found");
        break;
        case BME280::ChipModel_BMP280:
            Logger::info("-> Sensor found");
            Logger::warn("-> Humidity not supported");
        break;
        default:
            Logger::error("-> Sensor error");
    }
};

void SensorBME280::loop(SensorsData* data) {
    float pressure(NAN), temperature(NAN), humidity(NAN);
    Logger::info("Refreshing BME280");
    sensor.read(pressure, temperature, humidity, temperatureUnit, pressureUnit);
    Logger::debug("-> pressure:\t" + String(pressure));
    Logger::debug("-> temperature:\t" + String(temperature));
    Logger::debug("-> humidity:\t" + String(humidity));
};

#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorBME280.h>

#include "BME280I2C.h"

BME280I2C sensorAirBME280;
BME280::TempUnit temperatureUnit(BME280::TempUnit_Celsius);
BME280::PresUnit pressureUnit(BME280::PresUnit_Pa);

void SensorBME280::initialize() {
    Logger::info("Initializing BME280");
    if (!sensorAirBME280.begin()) {
        Logger::warn("BMC280 is not responding");
    }
    switch(sensorAirBME280.chipModel()) {
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
    Logger::info("Refreshing BME280");
    sensorAirBME280.read(data->pressure, data->temperature, data->humidity, temperatureUnit, pressureUnit);
    Logger::debug("-> pressure:\t" + String(data->pressure));
    Logger::debug("-> temperature:\t" + String(data->temperature));
    Logger::debug("-> humidity:\t" + String(data->humidity));
};

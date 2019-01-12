#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorBME280.h>

#include "BME280I2C.h"

BME280I2C sensorAirBME280;
BME280::TempUnit temperatureUnit(BME280::TempUnit_Celsius);
BME280::PresUnit pressureUnit(BME280::PresUnit_Pa);

Timer<1> SensorBME280::sensorReadTimer;

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
    SensorBME280::sensorReadTimer.every(Configurations::SENSORS_BME280_REFRESH_INTERVAL, SensorBME280::refresh);
};

void SensorBME280::loop() {
    SensorBME280::sensorReadTimer.tick();
};

bool SensorBME280::refresh(void *) {
    Logger::info("Refreshing BME280");
    sensorAirBME280.read(Sensors::data.pressure, Sensors::data.temperature, Sensors::data.humidity, temperatureUnit, pressureUnit);
    Logger::debug("-> pressure:\t" + String(Sensors::data.pressure));
    Logger::debug("-> temperature:\t" + String(Sensors::data.temperature));
    Logger::debug("-> humidity:\t" + String(Sensors::data.humidity));
    return true;
}

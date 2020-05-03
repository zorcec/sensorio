/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorDS18B20.h>
#include <sensors/sensorBME280.h>

#include "BME280I2C.h"

bool SensorBME280::isActive = false;

BME280I2C::Settings bme280Settings(
    BME280::OSR_X16, // Temperature
    BME280::OSR_X16, // Humidity
    BME280::OSR_X1, // Pressure
    BME280::Mode_Normal,
    BME280::StandbyTime_1000ms,
    BME280::Filter_16,
    BME280::SpiEnable_False,
    BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);

BME280I2C sensorAirBME280(bme280Settings);
BME280::TempUnit temperatureUnit(BME280::TempUnit_Celsius);
BME280::PresUnit pressureUnit(BME280::PresUnit_Pa);

void SensorBME280::initialize() {
    Logger::info("Initializing BME280");
    if (!sensorAirBME280.begin()) {
        Logger::warn("BME280 is not responding");
    }
    switch(sensorAirBME280.chipModel()) {
        case BME280::ChipModel_BME280:
            Logger::info("-> Sensor found");
            SensorBME280::isActive = true;
        break;
        case BME280::ChipModel_BMP280:
            Logger::info("-> Sensor found");
            Logger::warn("-> Humidity not supported");
            SensorBME280::isActive = true;
        break;
        default:
            Logger::error("-> Sensor error");
    }
    SensorBME280::refresh();
};

void SensorBME280::loop() {

};

bool SensorBME280::refresh() {
    if (SensorBME280::isActive) {
        Logger::trace("Refreshing BME280");
        if (SensorDS18B20::isActive) {
            // DS has the priority and we will not read temperature
            float ignore;
            sensorAirBME280.read(Sensors::data.pressure, ignore, Sensors::data.humidity, temperatureUnit, pressureUnit);
        } else {
            sensorAirBME280.read(Sensors::data.pressure, Sensors::data.temperature, Sensors::data.humidity, temperatureUnit, pressureUnit);
        }
        Sensors::data.temperature += Configurations::data.TEMPERATURE_OFFSET;
        Logger::trace("-> pressure:\t" + String(Sensors::data.pressure));
        Logger::trace("-> temperature:\t" + String(Sensors::data.temperature));
        Logger::trace("-> humidity:\t" + String(Sensors::data.humidity));
        return true;
    }
    return false;
}

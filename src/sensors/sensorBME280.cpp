#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorBME280.h>

#include "BME280I2C.h"

BME280I2C::Settings bme280Settings(
    BME280::OSR_X16, // Temperature
    BME280::OSR_X16, // Humidity
    BME280::OSR_X16, // Pressure
    BME280::Mode_Forced,
    BME280::StandbyTime_1000ms,
    BME280::Filter_16,
    BME280::SpiEnable_False,
    BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);

BME280I2C sensorAirBME280(bme280Settings);
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
    SensorBME280::refresh();
    SensorBME280::sensorReadTimer.every(
        Configurations::data.REFRESH_INTERVALS.BME280, 
        [](void*) -> bool { return SensorBME280::refresh(); }
    );
};

void SensorBME280::loop() {
    SensorBME280::sensorReadTimer.tick();
};

bool SensorBME280::refresh() {
    Logger::trace("Refreshing BME280");
    sensorAirBME280.read(Sensors::data.pressure, Sensors::data.temperature, Sensors::data.humidity, temperatureUnit, pressureUnit);
    Sensors::data.temperature += Configurations::data.TEMPERATURE_OFFSET;
    Logger::trace("-> pressure:\t" + String(Sensors::data.pressure));
    Logger::trace("-> temperature:\t" + String(Sensors::data.temperature));
    Logger::trace("-> humidity:\t" + String(Sensors::data.humidity));
    return true;
}

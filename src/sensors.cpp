#include <Wire.h>
#include <configurations.h>
#include <logger.h>
#include <sensors.h>
#include <BME280I2C.h>

float temperature(NAN), humidity(NAN), pressure(NAN);

BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_Off,
   BME280::SpiEnable_False,
   BME280I2C::I2CAddr_0x76 // I2C address. I2C specific.
);

BME280::TempUnit temperatureUnit(BME280::TempUnit_Celsius);
BME280::PresUnit pressureUnit(BME280::PresUnit_Pa);

BME280I2C bme(settings);

void Sensors::initialize() {
    Logger::log("Sensors are initializing");
    Sensors::startBME280();
}

void Sensors::startBME280() {
    switch(bme.chipModel()) {
        case BME280::ChipModel_BME280:
            Logger::log("Found BME280 sensor");
        break;
        case BME280::ChipModel_BMP280:
            Logger::log("Found BME280 sensor with no humidity available");
        break;
        default:
            Logger::log("Found UNKNOWN BME sensor");
    }
    settings.tempOSR = BME280::OSR_X4;
    bme.setSettings(settings);
}

void Sensors::readBME280() {
   float temperature(NAN), humidity(NAN), pressure(NAN);
   bme.read(pressure, temperature, humidity, temperatureUnit, pressureUnit);
};

void Sensors::loop() {
    Sensors::readBME280();
    Logger::log("Refreshing BME280");
    Logger::log("Temperature: " + String(temperature));
    Logger::log("Pressure: " + String(pressure));
    Logger::log("Humidity: " + String(humidity));
    delay(1000);
}

float Sensors::getTemperature() {
    return temperature;
}

float Sensors::getHumidity() {
    return humidity;
}

float Sensors::getPressure() {
    return pressure;
}
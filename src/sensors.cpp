/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <Wire.h>
#include <configurations.h>
#include <logger.h>
#include <sensors.h>
#include <sensors/sensorNative.h>
#include <sensors/sensorBME280.h>
#include <sensors/sensorTSL2561.h>
#include <sensors/sensorPIR.h>
#include <sensors/sensorMQ135.h>
#include <sensors/sensorCDS1.h>
#include <sensors/sensorDS18B20.h>
#include <permanentStorage.h>
#include <airQuality.h>

SensorsData Sensors::data {};
int Sensors::activeSensors[SUPORTED_I2C_SENSORS_NR] {};

void Sensors::initialize() {
    Logger::info("Sensors are initializing");
    Sensors::startI2c();
};

void Sensors::startI2c() {
    int sensors[127];
    Logger::info("-> Starting I2c");
    Logger::debug(" -> SDA GPIO" + String(Configurations::data.I2C_SDA));
    Logger::debug(" -> SCL GPIO" + String(Configurations::data.I2C_SCL));
    Wire.begin(Configurations::data.I2C_SDA, Configurations::data.I2C_SCL);
    Sensors::scan(sensors);
    Sensors::start(sensors);
};

void Sensors::start(int* sensors) {
    int sensorsCount = 0;
    SensorNative::initialize();
    if (sensors[57] == 0) {
        SensorTSL2561::initialize();
        Sensors::activeSensors[sensorsCount] = SensorTypes::LIGHT_TSL2561;
        sensorsCount++;
    }
    if (sensors[118] == 0) {
        SensorBME280::initialize();
        Sensors::activeSensors[sensorsCount] = SensorTypes::AIR_BME280;
        sensorsCount++;
    }
    if (Configurations::data.SENSORS_PIR_GPIO != NAN) { 
        SensorPIR::initialize();
        sensorsCount++;
    }
    if (Configurations::data.DS18B20_GPIO > 0) {
        SensorDS18B20::initialize();
    }
    switch(Configurations::data.SENSOR_ANALOG) {
        Logger::info("Initializing analog sensor");
        case SensorAnalogTypes::MQ135:
            SensorMQ135::initialize();
            sensorsCount++;
            break;
        case SensorAnalogTypes::CDS1:
            SensorCDS1::initialize();
            sensorsCount++;
            break;
        default:
            Logger::info("-> Analog sensor is not active");
    }
    Logger::info("Number of sensors started: " + String(sensorsCount + 1));
}

void Sensors::scan(int* sensors) {
    byte error, address;
    Logger::info("-> Scan started");
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        sensors[address] = error;
        if (error == 0) {
            Logger::debug(" -> Sensor found at address: " + String(address));
        } else if (error == 4) {
            Logger::warn(" -> Unknown error at address: " + String(address));
        }
    }
    Logger::info("-> Scan finished");
};

bool Sensors::isSensorActive(SensorTypes sensorType) {
    for (int index = 0; index < SUPORTED_I2C_SENSORS_NR; index++) {
        if (Sensors::activeSensors[index] == sensorType) {
            return true;
        }
    }
    return false;
}

void Sensors::refreshData() {
    SensorNative::refresh();
    SensorBME280::refresh();
    SensorTSL2561::refresh();
    SensorMQ135::refresh();
    SensorCDS1::refresh();
    SensorDS18B20::refresh();
    AirQuality::calculate();
}

void Sensors::loop() {
    SensorNative::loop();
    SensorBME280::loop();
    SensorTSL2561::loop();
    SensorPIR::loop();
    SensorMQ135::loop();
    SensorCDS1::loop();
    SensorDS18B20::loop();
};

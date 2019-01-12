#include <Wire.h>

#include <configurations.h>
#include <logger.h>
#include <sensors.h>
#include <sensors/sensorBME280.h>
#include <sensors/sensorTSL2561.h>

#define SUPORTED_SENSORS_NR 2

int activeSensors[SUPORTED_SENSORS_NR]; 
SensorsData data { NAN, NAN, NAN };

void Sensors::initialize() {
    Logger::info("Sensors are initializing");
    Sensors::startI2c();
};

void Sensors::startI2c() {
    int sensors[127];
    Logger::info("-> Starting I2c");
    Wire.begin(Configurations::I2C_SDA, Configurations::I2C_SCL);
    Sensors::scan(sensors);
    Sensors::start(sensors);
};

void Sensors::start(int* sensors) {
    int sensorsCount = 0;
    if (sensors[57] == 0) {
        activeSensors[sensorsCount] = SensorTypes::LIGHT_TSL2561;
        sensorsCount++;
    }
    if (sensors[118] == 0) {
        SensorBME280::initialize();
        activeSensors[sensorsCount] = SensorTypes::AIR_BME280;
        sensorsCount++;
    }
    Logger::info("Number of sensors started: " + String(sensorsCount));
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
    for (int index = 0; index < SUPORTED_SENSORS_NR; index++) {
        if (activeSensors[index] == sensorType) {
            return true;
        }
    }
    return false;
}

void Sensors::loop() {
    if (Sensors::isSensorActive(SensorTypes::AIR_BME280)) {
        SensorBME280::loop(&data);
    }
    if (Sensors::isSensorActive(SensorTypes::LIGHT_TSL2561)) {
        SensorTSL2561::loop(&data);
    }
    Logger::debug("All sensors refreshed");
};

SensorsData* Sensors::getData() {
    return &data;
};

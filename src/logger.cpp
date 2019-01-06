#include <Arduino.h>
#include <configurations.h>
#include <logger.h>

bool Logger::isSerialReady = false;

void Logger::initialize() {
    if (Configurations::SERIAL_LOGGING) {
        Serial.begin(115200);
        Serial.println();
        isSerialReady = true;
        Logger::log("Serial logging activated");
    }
    Logger::log("Logger initialized");
};

void Logger::log(String message) {
    if (isSerialReady) {
        Serial.print(message);
        Serial.println();
    }
};

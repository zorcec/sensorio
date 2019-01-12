#include <Arduino.h>
#include <configurations.h>
#include <logger.h>

bool Logger::isSerialReady = false;

void Logger::initialize() {
    if (Configurations::SERIAL_LOGGING) {
        Serial.begin(115200);
        Serial.println();
        isSerialReady = true;
        Logger::info("Serial logging activated");
    }
    Logger::debug("Logger initialized");
};

void Logger::log(String message) { 
    Logger::log(LogType::INFO, message);
}

void Logger::log(LogType type, String message) {
    String fullMessage = "[{TYPE}]\t{MESSAGE}";
    String typeStr = "";
    switch(type) {
        case LogType::DEBUG:
            typeStr = "DEBUG";
            break;
        case LogType::INFO:
            typeStr = "INFO";
            break;
        case LogType::WARN:
            typeStr = "WARN";
            break;
        case LogType::ERROR:
            typeStr = "ERROR";
            break;
    }
    fullMessage.replace("{TYPE}", typeStr);
    fullMessage.replace("{MESSAGE}", message);
    if (isSerialReady) {
        Serial.print(fullMessage);
        Serial.println();
    }
};

void Logger::debug(String message) {
    if (Configurations::DEBUG) {
        Logger::log(message);
    }
};

void Logger::info(String message) {
    Logger::log(message);
};

void Logger::warn(String message) {
    Logger::log(message);
};

void Logger::error(String message) {
    Logger::log(LogType::ERROR, message);
};

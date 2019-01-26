#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <sensors.h>
#include <logger.h>
#include <ArduinoJson.h>

struct RefreshIntervals {
    int16_t native;
    int16_t BME280;
    int16_t TSL2561;
};

struct LedGPIO {
    uint8_t RED;
    uint8_t GREEN;
    uint8_t BLUE;
};

struct ConfigurationData {
    String ID;
    String NAME;
    String VERSION;
    bool SERIAL_LOGGING;
    LogType LOGGING_LEVEL;
    int16_t SLEEP_INTERVAL;
    int16_t SLEEP_TIME;

    String WIFI_SSID;
    String WIFI_PASSWORD;
    int16_t WIFI_RECONNECT_TIME;

    String MQTT_SERVER;
    int16_t MQTT_PORT;
    String MQTT_TOPIC;
    int16_t MQTT_RECONNECT_TIME;
    int16_t MQTT_SEND_DATA_INTERVAL;
    String MQTT_TOPIC_NOTIFY;
    String MQTT_TOPIC_DATA;
    String MQTT_TOPIC_STATUS;
    String MQTT_TOPIC_CONFIGURATION;
    String MQTT_TOPIC_ADMINISTRATION;

    int8 I2C_SDA;
    int8 I2C_SCL;

    int8_t SENSORS_PIR_GPIO;

    RefreshIntervals REFRESH_INTERVALS;
    SensorsData SEND_DATA_DIFFERENCES;

    LedGPIO LED_GPIO;
};

class Configurations {

    public:
        static void initialize();
        static void onMessage(JsonObject& data);
        static ConfigurationData data;

};

#endif

/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <sensors.h>
#include <logger.h>
#include <ArduinoJson.h>

enum DeviceType {
    BOX,
    WALL_MOUNT,
    FINDER
};

struct LedGPIO {
    uint8_t RED;
    uint8_t GREEN;
    uint8_t BLUE;
};

struct ConfigurationData {
    DeviceType DEVICE_TYPE;
    String ID;
    String NAME;
    String VERSION;
    bool SERIAL_LOGGING;
    bool MQTT_LOGGING;
    LogType LOGGING_LEVEL;
    int16_t SLEEP_INTERVAL;
    int16_t SLEEP_TIME;
    int16_t NOTIFICATIONS_LED_BLINK_TIME;
    int8_t NOTIFICATIONS_LED_ITENSITY;

    String WIFI_SSID;
    String WIFI_PASSWORD;

    String MQTT_SERVER;
    int16_t MQTT_PORT;
    String MQTT_TOPIC;
    int16_t MQTT_RECONNECT_TIME;
    int16_t MQTT_SEND_DATA_INTERVAL;
    String MQTT_TOPIC_LOGGING;
    String MQTT_TOPIC_NOTIFICATIONS;
    String MQTT_TOPIC_DATA;
    String MQTT_TOPIC_STATUS;
    String MQTT_TOPIC_CONFIGURATION;
    String MQTT_TOPIC_ADMINISTRATION;

    int8 I2C_SDA;
    int8 I2C_SCL;

    int8_t SENSORS_PIR_GPIO;
    SensorAnalogTypes SENSOR_ANALOG;
    float_t TEMPERATURE_OFFSET;
    int16_t POLUTION_VALUE_OFFSET;
    float_t AIR_QUALITY_MIN;
    float_t AIR_QUALITY_MAX;

    int8 DS18B20_GPIO;

    LedGPIO LED_GPIO;

    int16_t SEND_PROBES_INTERVAL;
    String FIND_FAMILY;
    bool PASSIVE_TRACKING;
    String FIND_SERVER;
    String FIND_LOCATION;
};

class Configurations {

    public:
        static void initialize();
        static void onMessage(JsonObject& data);
        static ConfigurationData data;

};

#endif

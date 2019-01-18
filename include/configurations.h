#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Arduino.h>
#include <sensors.h>
#include <logger.h>

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

class Configurations {
    public:
        static void initialize();
        static String ID;
        static String NAME;
        static String VERSION;
        static bool SERIAL_LOGGING;
        static LogType LOGGING_LEVEL;
        static int8_t SLEEP;

        static String WIFI_SSID;
        static String WIFI_PASSWORD;
        static int16_t WIFI_RECONNECT_TIME;

        static String MQTT_SERVER;
        static int16_t MQTT_PORT;
        static String MQTT_TOPIC;
        static int16_t MQTT_RECONNECT_TIME;
        static int16_t MQTT_SEND_DATA_INTERVAL;

        static int8 I2C_SDA;
        static int8 I2C_SCL;

        static int8_t SENSORS_PIR_GPIO;

        static RefreshIntervals REFRESH_INTERVALS;
        static SensorsData SEND_DATA_DIFFERENCES;

        static LedGPIO LED_GPIO;

};

#endif

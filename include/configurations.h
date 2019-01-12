#include <Arduino.h>

class Configurations {
    public:
        static void initialize();
        static String ID;
        static String NAME;
        static String VERSION;
        static bool SERIAL_LOGGING;
        static bool DEBUG;

        static String WIFI_SSID;
        static String WIFI_PASSWORD;
        static int16_t WIFI_RECONNECT_TIME;

        static String MQTT_SERVER;
        static int MQTT_PORT;
        static String MQTT_TOPIC;
        static int16_t MQTT_RECONNECT_TIME;
        static int16_t MQTT_SEND_DATA_INTERVAL;

        static float_t SEND_DATA_TEMPERATURE_DIFFERENCE;
        static float_t SEND_DATA_PRESSURE_DIFFERENCE;
        static float_t SEND_DATA_HUMIDITY_DIFFERENCE;
        static float_t SEND_DATA_FULLSPECTRUM_DIFFERENCE;
        static float_t SEND_DATA_RSSI_DIFFERENCE;

        static int8 I2C_SDA;
        static int8 I2C_SCL;

        static int16_t SENSORS_NATIVE_REFRESH_INTERVAL;
        static int16_t SENSORS_BME280_REFRESH_INTERVAL;
        static int16_t SENSORS_TSL2561_REFRESH_INTERVAL;
};

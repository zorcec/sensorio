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
        static unsigned int WIFI_RECONNECT_TIME;

        static String MQTT_SERVER;
        static int MQTT_PORT;
        static String MQTT_TOPIC;
        static unsigned int MQTT_RECONNECT_TIME;

        static int8 I2C_SDA;
        static int8 I2C_SCL;

        static int32 SENSORS_REFRESH_RATE;
};

#include <Arduino.h>

class Configurations {
    public:
        static void initialize();
        static String ID;
        static String NAME;
        static String VERSION;
        static bool SERIAL_LOGGING;

        static String WIFI_SSID;
        static String WIFI_PASSWORD;

        static String MQTT_SERVER;
        static int MQTT_PORT;
        static String MQTT_TOPIC;
};

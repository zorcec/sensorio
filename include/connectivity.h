#include <Arduino.h>

class Connectivity {
    public:
        static void loop();
        static void sendStatus();
        static void sendMessage(String dataJson);
        static String getTopic(String name);
    private:
        static void autoconnectToWifi();
        static void autoconnectToMqtt();
};

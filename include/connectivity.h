#include <Arduino.h>

class Connectivity {
    public:
        static void loop();
        static void sendStatus();
        static void sendMessage(String);
        static String getTopic(String);
        static String askDevice(int8, byte);
        static void startI2c();

    private:
        static void autoconnectToWifi();
        static void autoconnectToMqtt();
};

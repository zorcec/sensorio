#include <Arduino.h>
#include <ArduinoJson.h>
#include <timer.h>

class Connectivity {
    public:
        static void initialize();
        static void loop();
        static void sendStatus();
        static bool autosendData(void *);
        static void sendData(bool);
        static void sendMessage(String);
        static String getTopic(String);
        static String askDevice(int8, byte);

    private:
        static void autoconnectToWifi();
        static void autoconnectToMqtt();
        static Timer<1> sendDataTimer;
};

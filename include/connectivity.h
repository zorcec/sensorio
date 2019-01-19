#include <Arduino.h>
#include <ArduinoJson.h>
#include <timer.h>
#include <sensors.h>

#define MESSAGE_SIZE 500

class Connectivity {
    public:
        static void initialize();
        static void loop();
        static void sendStatus();
        static bool autosendData(void *);
        static void sendData();
        static void sendJson(String, JsonObject&);
        static void sendEvent(String);
        static void sendMessage(String, String);
        static String getTopic(String);
        static String askDevice(int8, byte);
        static bool subscribe(String, bool);

    private:
        static void autoconnectToWifi();
        static void autoconnectToMqtt();
        static bool checkDiff();
        static Timer<1> sendDataTimer;
        static JsonObject& jsonData;
        static StaticJsonBuffer<MESSAGE_SIZE> jsonBuffer;
        static SensorsData sentData;
        static String subscribeTopicData;
        static String subscribeTopicStatus;
        static String subscribeTopicConfiguration;

};

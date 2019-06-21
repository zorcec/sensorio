#include <Arduino.h>
#include <ArduinoJson.h>
#include <timer.h>
#include <sensors.h>

#define MESSAGE_SIZE 512

typedef void (*callbackHandler_t)(JsonObject& data);

struct MqttCallback {
    String topic;
    callbackHandler_t callback;
};

class Connectivity {
    public:
        static void initialize();
        static void loop();
        static void sendStatus();
        static bool autosendData(void *);
        static void connectToWifi();
        static void sendData();
        static void sendJson(String, JsonObject&);
        static void sendEvent(String, String);
        static void sendMessage(String, String);
        static String getTopic(String);
        static String askDevice(int8, byte);
        static bool subscribe(String, callbackHandler_t);
        static void addMqttCallback(String, callbackHandler_t);
        static callbackHandler_t getMqttCallback(String);
        static void callbackNoop(JsonObject&);
        static DynamicJsonBuffer jsonBuffer;

    private:
        static void autoconnectToMqtt();
        static bool checkDiff();
        static void onMessageStatus(JsonObject&);
        static void onMessageData(JsonObject&);
        static void onMessageConfiguration(JsonObject&);
        static MqttCallback* mqttCallbacks;
        static uint8_t mqttCallbackCount;
        static Timer<1> sendDataTimer;
        static Timer<5> resubscribeTimer;
        static SensorsData sentData;
        static String subscribeTopicData;
        static String subscribeTopicStatus;
        static String subscribeTopicConfiguration;

};

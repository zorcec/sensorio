#include <Arduino.h>

class Connectivity {
    public:
        static void connectToWifi();
        static void connectToMqtt();
        static void loop();
        static void sendStatus();
        static void sendMessage(String dataJson);
        static String getTopic(String name);
    private:
        
};

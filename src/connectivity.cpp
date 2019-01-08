#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include "user_interface.h"

#include <connectivity.h>
#include <configurations.h>
#include <logger.h>

#define DATA_SIZE 500

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void Connectivity::sendStatus() {
    char dataJson[DATA_SIZE];
    StaticJsonBuffer<DATA_SIZE> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    // data to be sent
    data["name"] = Configurations::NAME;
    data["version"] = Configurations::VERSION;

    data.createNestedObject("info");
    data["info"]["memory"] = system_get_free_heap_size();
    data["info"]["ssid"] = Configurations::WIFI_SSID;
    data["info"]["rssi"] = WiFi.RSSI(); 

    data.printTo(dataJson);
    Connectivity::sendMessage(dataJson);
}

void Connectivity::sendMessage(String dataJson) {
    Logger::log("MQTT sending message: " + dataJson);
    client.beginPublish(Connectivity::getTopic("STATUS").c_str(), dataJson.length(), false);
    client.print(dataJson);
    client.endPublish();
}

String Connectivity::getTopic(String name) {
    String topic = "{TOPIC}/{ID}/{NAME}";
    topic.replace("{TOPIC}", Configurations::MQTT_TOPIC);
    topic.replace("{ID}", Configurations::ID);
    topic.replace("{NAME}", name);
    return topic;
}

void Connectivity::autoconnectToWifi() {
    if (WiFi.status() != WL_CONNECTED) {
        Logger::log("WiFi trying to connect");
        while (WiFi.status() != WL_CONNECTED) {
            if (WiFi.begin(Configurations::WIFI_SSID.c_str(), Configurations::WIFI_PASSWORD.c_str()) == WL_CONNECTED) {
                Logger::log("WiFI connected, IP address: " + WiFi.localIP());
            } else {
                Logger::log("WiFI connection is still not ready, will retry in 5s");
            }
            delay(Configurations::WIFI_RECONNECT_TIME);
        }
    }
}

void Connectivity::autoconnectToMqtt() {
    if (!client.connected()) {
        Logger::log("MQTT trying to connect");
        client.setServer(Configurations::MQTT_SERVER.c_str(), Configurations::MQTT_PORT);
        client.setCallback(callback);
        while (!client.connected()) {
            if (client.connect(Configurations::ID.c_str())) {
                Logger::log("MQTT connected");
            } else {
                Logger::log("MQTT connection is still not ready; will retry in 5s");
            }
            delay(Configurations::MQTT_RECONNECT_TIME);
        }
    }
}

void Connectivity::loop() {
    Connectivity::autoconnectToWifi();
    Connectivity::autoconnectToMqtt();
    client.loop();
}
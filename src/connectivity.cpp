#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "user_interface.h"

#include <connectivity.h>
#include <configurations.h>
#include <logger.h>
#include <sensors.h>

#define MESSAGE_SIZE 500

WiFiClient espClient;
PubSubClient client(espClient);

Timer<1> Connectivity::sendDataTimer;

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void Connectivity::initialize() {
    Logger::info("Initializing connectivity");
    Connectivity::sendDataTimer.every(Configurations::MQTT_SEND_DATA_INTERVAL, Connectivity::autosendData);
};

void Connectivity::sendStatus() {
    char dataJson[MESSAGE_SIZE];
    StaticJsonBuffer<MESSAGE_SIZE> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    // data to be sent
    data["name"] = Configurations::NAME;
    data["version"] = Configurations::VERSION;
    data["SSID"] = Configurations::WIFI_SSID;

    if (Configurations::DEBUG) {
        data.prettyPrintTo(dataJson);
    } else {
        data.printTo(dataJson);
    }
    Connectivity::sendMessage("STATUS", dataJson);
}

bool Connectivity::autosendData(void *) {
    Connectivity::sendData(true);
    return true;
}

void Connectivity::sendData(bool checkData) {
    if (checkData) {
        // TODO compare old and current data difference
    }

    char dataJson[MESSAGE_SIZE];
    StaticJsonBuffer<MESSAGE_SIZE> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();
  
    // data to be sent
    data["temperature"] = Sensors::data.temperature;
    data["pressure"] = Sensors::data.pressure;
    data["humidity"] = Sensors::data.humidity;
    data["visibleLight"] = Sensors::data.visibleLight;
    data["infraredLight"] = Sensors::data.infraredLight;
    data["fullSpectrumLight"] = Sensors::data.fullSpectrumLight;
    data["RSSI"] = Sensors::data.RSSI;

    if (Configurations::DEBUG) {
        data.prettyPrintTo(dataJson);
    } else {
        data.printTo(dataJson);
    }
    Connectivity::sendMessage("DATA", dataJson);
}

void Connectivity::sendMessage(String topic, String dataJson) {
    Logger::info("MQTT sending message: " + dataJson);
    client.beginPublish(Connectivity::getTopic(topic).c_str(), dataJson.length(), false);
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
        Logger::info("WiFi trying to connect");
        while (WiFi.status() != WL_CONNECTED) {
            if (WiFi.begin(Configurations::WIFI_SSID.c_str(), Configurations::WIFI_PASSWORD.c_str()) == WL_CONNECTED) {
                Logger::info("WiFI connected, IP address: " + WiFi.localIP());
            } else {
                Logger::error("WiFI connection is still not ready, will retry in 5s");
            }
            delay(Configurations::WIFI_RECONNECT_TIME);
        }
    }
}

void Connectivity::autoconnectToMqtt() {
    if (!client.connected()) {
        Logger::info("MQTT trying to connect");
        client.setServer(Configurations::MQTT_SERVER.c_str(), Configurations::MQTT_PORT);
        client.setCallback(callback);
        while (!client.connected()) {
            if (client.connect(Configurations::ID.c_str())) {
                Logger::info("MQTT connected");
                Connectivity::sendStatus();
            } else {
                Logger::info("MQTT connection is still not ready; will retry in 5s");
            }
            delay(Configurations::MQTT_RECONNECT_TIME);
        }
    }
}

void Connectivity::loop() {
    //Connectivity::autoconnectToWifi(); not needed, done automatically by the lib
    Connectivity::autoconnectToMqtt();
    Connectivity::sendDataTimer.tick();
    client.loop();
}
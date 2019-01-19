#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include "user_interface.h"
#include <cmath> 

#include <connectivity.h>
#include <configurations.h>
#include <logger.h>
#include <sensors.h>

WiFiClient espClient;
PubSubClient client(espClient);

Timer<1> Connectivity::sendDataTimer;
StaticJsonBuffer<MESSAGE_SIZE> Connectivity::jsonBuffer;
JsonObject& Connectivity::jsonData = jsonBuffer.createObject();
SensorsData Connectivity::sentData;

String Connectivity::subscribeTopicData = "/CMD/" + Connectivity::getTopic("DATA");
String Connectivity::subscribeTopicStatus = "/CMD/" + Connectivity::getTopic("STATUS");;
String Connectivity::subscribeTopicConfiguration = "/CMD/" + Connectivity::getTopic("CONFIGURATION");

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void Connectivity::initialize() {
    Logger::info("Initializing connectivity");
    Connectivity::sendDataTimer.every(Configurations::MQTT_SEND_DATA_INTERVAL, Connectivity::autosendData);
};

void Connectivity::sendStatus() {
    StaticJsonBuffer<MESSAGE_SIZE> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    // data to be sent
    data["name"] = Configurations::NAME;
    data["version"] = Configurations::VERSION;
    data["SSID"] = Configurations::WIFI_SSID;

    Connectivity::sendJson("STATUS", data);
}

bool Connectivity::autosendData(void *) {
    if(Connectivity::checkDiff()) {
        Connectivity::sendData();
    } else {
        Logger::trace("Not sending; difference is not large enough");
    }
    return true;
}

bool Connectivity::checkDiff() {
    SensorsData& sent = Connectivity::sentData;
    SensorsData& current = Sensors::data;
    SensorsData& diff = Configurations::SEND_DATA_DIFFERENCES;
    if (abs(current.temperature - sent.temperature) >= diff.temperature) {
        return true;
    }
    if (abs(current.fullSpectrumLight - sent.fullSpectrumLight) >= diff.fullSpectrumLight) {
        return true;
    }
    return false;
}

void Connectivity::sendEvent(String eventName) {
    StaticJsonBuffer<MESSAGE_SIZE> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    // data to be sent
    data["name"] = eventName;

    Connectivity::sendJson("EVENT", data);
}

void Connectivity::sendData() {
    JsonObject& json = Connectivity::jsonData;
    SensorsData& sent = Connectivity::sentData;

    // data to be sent
    json["temperature"]         = sent.temperature          = Sensors::data.temperature;
    json["pressure"]            = sent.pressure             = Sensors::data.pressure;
    json["humidity"]            = sent.humidity             = Sensors::data.humidity;
    json["visibleLight"]        = sent.visibleLight         = Sensors::data.visibleLight;
    json["infraredLight"]       = sent.infraredLight        = Sensors::data.infraredLight;
    json["fullSpectrumLight"]   = sent.fullSpectrumLight    = Sensors::data.fullSpectrumLight;
    json["RSSI"]                = sent.RSSI                 = Sensors::data.RSSI;

    Connectivity::sendJson("DATA", Connectivity::jsonData);
}

void Connectivity::sendJson(String topic, JsonObject& dataJson) {
    char jsonMessage[MESSAGE_SIZE];
    if (Configurations::LOGGING_LEVEL == LogType::DEBUG) {
        dataJson.prettyPrintTo(jsonMessage);
    } else {
        dataJson.printTo(jsonMessage);
    }
    Connectivity::sendMessage(topic, jsonMessage);
}

void Connectivity::sendMessage(String topic, String message) {
    String fullTopic = Connectivity::getTopic(topic).c_str();
    Logger::info("MQTT sending to: " + fullTopic + ", message: " + message);
    client.beginPublish(fullTopic.c_str(), message.length(), false);
    client.print(message);
    client.endPublish();
}

String Connectivity::getTopic(String name) {
    String topic = "/{TOPIC}/{ID}/{NAME}";
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
                Connectivity::subscribe(Connectivity::subscribeTopicStatus, false);
                Connectivity::subscribe(Connectivity::subscribeTopicData, false);
                Connectivity::subscribe(Connectivity::subscribeTopicConfiguration, false);
                Logger::debug("-> waiting 1s");
                delay(1000);
                Connectivity::sendStatus();
            } else {
                Logger::info("MQTT connection is still not ready; will retry in 5s");
            }
            delay(Configurations::MQTT_RECONNECT_TIME);
        }
    }
}

bool Connectivity::subscribe(String topic, bool custom) {
    if (client.connected()) {
        String fullTopic;
        if (custom) {
            fullTopic = topic;
        } else {
            fullTopic = "/cmd" + Connectivity::getTopic(topic);
        }
        client.subscribe(fullTopic.c_str());
        Logger::info("MQTT subscription: " + String(topic));
        return true;
    }
    return false;
}

void Connectivity::loop() {
    //Connectivity::autoconnectToWifi(); not needed, done automatically by the MQTT lib
    Connectivity::autoconnectToMqtt();
    Connectivity::sendDataTimer.tick();
    client.loop();
}
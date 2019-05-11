#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <Wire.h>
#include "user_interface.h"
#include <cmath> 
#include <Esp.h>

#include <connectivity.h>
#include <configurations.h>
#include <logger.h>
#include <sensors.h>
#include <permanentStorage.h>
#include <airQuality.h>

WiFiClient espClient;
PubSubClient client(espClient);

Timer<1> Connectivity::sendDataTimer;
StaticJsonBuffer<MESSAGE_SIZE> Connectivity::jsonBuffer;
JsonObject& Connectivity::jsonData = jsonBuffer.createObject();
SensorsData Connectivity::sentData;

MqttCallback* Connectivity::mqttCallbacks = new MqttCallback[10];
uint8_t Connectivity::mqttCallbackCount = 0;

void Connectivity::initialize() {
    Logger::info("Initializing connectivity");
    Logger::debug("-> waiting for 2s");
    delay(2000);
    Connectivity::loop();
    Connectivity::sendDataTimer.every(Configurations::data.MQTT_SEND_DATA_INTERVAL, Connectivity::autosendData);
}

void Connectivity::onMessageStatus(JsonObject& data) {
    Logger::info("Connectivity received STATUS comand");
    Connectivity::sendStatus();
}

void Connectivity::onMessageData(JsonObject& data) {
    Logger::info("Connectivity received DATA comand");
    Connectivity::sendData();
}

void Connectivity::onMessageConfiguration(JsonObject& data) {
    Logger::info("Connectivity received CONFIGURATION comand");
}

void Connectivity::sendStatus() {
    StaticJsonBuffer<MESSAGE_SIZE> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    // data to be sent
    data["name"] = Configurations::data.NAME;
    data["version"] = Configurations::data.VERSION;
    data["SSID"] = Configurations::data.WIFI_SSID;

    data["info"] = data.createNestedObject("info");
    data["info"]["cpu"] = ESP.getCpuFreqMHz();
    data["info"]["heap"] = ESP.getFreeHeap();
    data["info"]["reset_reason"] = ESP.getResetReason();

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
    SensorsData& diff = Configurations::data.SEND_DATA_DIFFERENCES;
    if (fabs(current.temperature - sent.temperature) >= diff.temperature) {
        return true;
    } else if (abs(current.pressure - sent.pressure) >= diff.pressure) {
        return true;
    } else if (fabs(current.humidity - sent.humidity) >= diff.humidity) {
        return true;
    } else if (abs(current.RSSI - sent.RSSI) >= diff.RSSI) {
        return true;
    } else if (abs(current.fullSpectrumLight - sent.fullSpectrumLight) >= diff.fullSpectrumLight) {
        return true;
    } else if (fabs(current.airQuality - sent.airQuality) >= diff.airQuality) {
        return true;
    } else if (abs(current.analogValue - sent.analogValue) >= diff.analogValue) {
        return true;
    }
    return false;
}

void Connectivity::sendEvent(String eventName, String eventData) {
    StaticJsonBuffer<MESSAGE_SIZE> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();

    // data to be sent
    data["data"] = eventData;

    Connectivity::sendJson("EVENT/" + eventName, data);
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
    json["analogValue"]         = sent.analogValue          = Sensors::data.analogValue;
    json["brightness"]          = sent.brightness           = Sensors::data.brightness;
    json["airQuality"]          = sent.airQuality           = Sensors::data.airQuality;
    json["airQualityMin"]       = AirQuality::airQualityMin;
    json["airQualityMax"]       = AirQuality::airQualityMax;
    json["airQualityAdaptive"]  = AirQuality::airQualityAdaptive;

    Connectivity::sendJson("DATA", Connectivity::jsonData);
}

void Connectivity::sendJson(String topic, JsonObject& dataJson) {
    char jsonMessage[MESSAGE_SIZE];
    if (Configurations::data.LOGGING_LEVEL == LogType::DEBUG) {
        dataJson.prettyPrintTo(jsonMessage);
    } else {
        dataJson.printTo(jsonMessage);
    }
    Connectivity::sendMessage(topic, jsonMessage);
}

void Connectivity::sendMessage(String topic, String message) {
    String fullTopic = Connectivity::getTopic(topic).c_str();
    if (!Configurations::data.MQTT_LOGGING) {
        Logger::info("MQTT sending to: " + fullTopic + ", message: " + message);
    }
    client.beginPublish(fullTopic.c_str(), message.length(), false);
    client.print(message);
    client.endPublish();
}

String Connectivity::getTopic(String name) {
    String topic = "/{TOPIC}/{ID}/{NAME}";
    topic.replace("{TOPIC}", Configurations::data.MQTT_TOPIC);
    topic.replace("{ID}", Configurations::data.ID);
    topic.replace("{NAME}", name);
    return topic;
}


void Connectivity::addMqttCallback(String topic, callbackHandler_t callback) {
    Connectivity::mqttCallbacks[Connectivity::mqttCallbackCount] = MqttCallback { topic, callback };
    Connectivity::mqttCallbackCount++;
}

callbackHandler_t Connectivity::getMqttCallback(String topic) {
    MqttCallback* current;
    for (uint8_t index = 0; index < Connectivity::mqttCallbackCount; index++) {
        current = &Connectivity::mqttCallbacks[index];
        if (current->topic == topic) {
            return current->callback;
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    StaticJsonBuffer<500> jsonBuffer;   
    String topicStr = String(topic);
    payload[length] = '\0'; // will make sure payload is clean
    String message = (char*)payload;
    JsonObject& jsonObj = jsonBuffer.parseObject(message);

    Logger::info("Received message on: " + topicStr);
    if (Configurations::data.LOGGING_LEVEL <= LogType::DEBUG) {
        String data;
        jsonObj.prettyPrintTo(data);
        Logger::debug("-> " + data);
    }
    callbackHandler_t callback = Connectivity::getMqttCallback(topicStr);
    callback(jsonObj);
}

void Connectivity::autoconnectToWifi() {
    if (WiFi.status() != WL_CONNECTED) {
        Logger::info("WiFi trying to connect");
        while (WiFi.status() != WL_CONNECTED) {
            if (WiFi.begin(Configurations::data.WIFI_SSID.c_str(), Configurations::data.WIFI_PASSWORD.c_str()) == WL_CONNECTED) {
                Logger::info("WiFI connected");
            } else {
                Logger::error("WiFI connection is still not ready, will retry in 5s");
            }
            delay(Configurations::data.WIFI_RECONNECT_TIME);
        }
    }
}

void Connectivity::autoconnectToMqtt() {
    if (!client.connected()) {
        Logger::info("MQTT trying to connect");
        client.setServer(Configurations::data.MQTT_SERVER.c_str(), Configurations::data.MQTT_PORT);
        client.setCallback(callback);
        while (!client.connected()) {
            if (client.connect(Configurations::data.ID.c_str())) {
                Logger::info("MQTT connected");
                Connectivity::subscribe(Configurations::data.MQTT_TOPIC_STATUS, Connectivity::onMessageStatus);
                Connectivity::subscribe(Configurations::data.MQTT_TOPIC_DATA, Connectivity::onMessageData);
                Connectivity::subscribe(Configurations::data.MQTT_TOPIC_CONFIGURATION, Connectivity::callbackNoop);
                Logger::debug("-> waiting 1s");
                delay(1000);
                Connectivity::sendStatus();
            } else {
                Logger::info("MQTT connection is still not ready; will retry in 5s");
            }
            delay(Configurations::data.MQTT_RECONNECT_TIME);
        }
    }
}

bool Connectivity::subscribe(String topic, callbackHandler_t callback) {
    if (client.connected()) {
        String fullTopic;
        if (topic[0] == '/') {
            fullTopic = topic;
        } else {
            fullTopic = "/cmd" + Connectivity::getTopic(topic);
        }
        client.subscribe(fullTopic.c_str());
        Connectivity::addMqttCallback(fullTopic, callback);
        Logger::info("MQTT subscription: " + String(fullTopic));
        return true;
    } else {
        Logger::warn("Cannot subscribe yet: " + topic);
        Logger::debug("-> trying to reconnect");
        Connectivity::autoconnectToMqtt();
        Connectivity::subscribe(topic, callback);
    }
    return false;
}

void Connectivity::callbackNoop(JsonObject& message) {
    String data;
    message.prettyPrintTo(data);
    Logger::debug("Noop message: " + data);
}

void Connectivity::loop() {
    Connectivity::autoconnectToWifi();
    Connectivity::autoconnectToMqtt();
    Connectivity::sendDataTimer.tick();
    client.loop();
}

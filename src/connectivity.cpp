/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <connectivity.h>
#include <configurations.h>
#include <logger.h>
#include <sensors.h>
#include <permanentStorage.h>
#include <airQuality.h>

WiFiClient Connectivity::espClient;
PubSubClient Connectivity::client(espClient);
HTTPClient Connectivity::http;

Timer<1> Connectivity::sendDataTimer;
DynamicJsonBuffer Connectivity::jsonBuffer;

MqttCallback* Connectivity::mqttCallbacks = new MqttCallback[25];
uint8_t Connectivity::mqttCallbackCount = 0;

void Connectivity::initialize() {
    Logger::info("Initializing connectivity");
    Logger::debug("-> waiting for 2s");
    delay(2000);
    Connectivity::connectToWifi();
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
    JsonObject& data = Connectivity::jsonBuffer.createObject();

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
    Sensors::refreshData();
    Connectivity::sendData();
    return true;
}

void Connectivity::sendEvent(String eventName, String eventData) {
    JsonObject& data = Connectivity::jsonBuffer.createObject();

    // data to be sent
    data["data"] = eventData;

    Connectivity::sendJson("EVENT/" + eventName, data);
}

void Connectivity::sendData() {
    JsonObject& json = Connectivity::jsonBuffer.createObject();

    // data to be sent
    json["temperature"]         = Sensors::data.temperature;
    json["pressure"]            = Sensors::data.pressure;
    json["humidity"]            = Sensors::data.humidity;
    json["visibleLight"]        = Sensors::data.visibleLight;
    json["infraredLight"]       = Sensors::data.infraredLight;
    json["fullSpectrumLight"]   = Sensors::data.fullSpectrumLight;
    json["RSSI"]                = Sensors::data.RSSI;
    json["analogValue"]         = Sensors::data.analogValue;
    json["brightness"]          = Sensors::data.brightness;
    json["airQuality"]          = Sensors::data.airQuality;
    json["airQualityMin"]       = AirQuality::airQualityMin;
    json["airQualityMax"]       = AirQuality::airQualityMax;
    json["airQualityAdaptive"]  = AirQuality::airQualityAdaptive;

    Connectivity::sendJson("DATA", json);
}

void Connectivity::sendJson(String topic, JsonObject& dataJson) {
    String jsonMessage;
    if (Configurations::data.LOGGING_LEVEL <= LogType::DEBUG) {
        dataJson.prettyPrintTo(jsonMessage);
    } else {
        dataJson.printTo(jsonMessage);
    }
    Connectivity::sendMessage(topic, jsonMessage);
}

void Connectivity::sendJsonHttp(String url, JsonObject& dataJson) {
    String jsonMessage;
    if (Configurations::data.LOGGING_LEVEL <= LogType::DEBUG) {
        dataJson.prettyPrintTo(jsonMessage);
    } else {
        dataJson.printTo(jsonMessage);
    }
    Logger::info("HTTP POST to: " + url + ", message: " + jsonMessage);
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.POST(jsonMessage);
    http.end();
}

void Connectivity::sendMessage(String topic, String message) {
    String fullTopic = Connectivity::getTopic(topic).c_str();
    if (!Configurations::data.MQTT_LOGGING) {
        Logger::info("MQTT sending to: " + fullTopic + ", message: " + message);
    }
    Connectivity::client.beginPublish(fullTopic.c_str(), message.length(), false);
    Connectivity::client.print(message);
    Connectivity::client.endPublish();
}

String Connectivity::getTopic(String name) {
    String topic = "{TOPIC}/{ID}/{NAME}";
    topic.replace("{TOPIC}", Configurations::data.MQTT_TOPIC);
    topic.replace("{ID}", Configurations::data.ID);
    topic.replace("{NAME}", name);
    return topic;
}


void Connectivity::addMqttCallback(String topic, callbackHandler_t callback) {
    Connectivity::mqttCallbacks[Connectivity::mqttCallbackCount] = MqttCallback { topic, callback };
    Connectivity::mqttCallbackCount++;
}

callbackHandler_t ICACHE_RAM_ATTR Connectivity::getMqttCallback(String topic) {
    MqttCallback* current;
    for (uint8_t index = 0; index < Connectivity::mqttCallbackCount; index++) {
        current = &Connectivity::mqttCallbacks[index];
        if (current->topic == topic) {
            return current->callback;
        }
    }
    return NULL;
}

void ICACHE_RAM_ATTR callback(char* topic, byte* payload, unsigned int length) {
    String topicStr = String(topic);
    payload[length] = '\0'; // will make sure payload is clean
    String message = (char*)payload;
    JsonObject& jsonObj = Connectivity::jsonBuffer.parseObject(message);

    Logger::info("Received message on: " + topicStr);
    if (Configurations::data.LOGGING_LEVEL <= LogType::DEBUG) {
        String data;
        jsonObj.prettyPrintTo(data);
        Logger::debug("-> " + data);
    }
    callbackHandler_t callback = Connectivity::getMqttCallback(topicStr);
    if (callback != NULL) {
        callback(jsonObj);
    }
}

void Connectivity::connectToWifi() {
    if (WiFi.status() != WL_CONNECTED) {
        Logger::info("WiFi trying to connect");
        while (WiFi.status() != WL_CONNECTED) {
            if (WiFi.begin(Configurations::data.WIFI_SSID.c_str(), Configurations::data.WIFI_PASSWORD.c_str()) == WL_CONNECTED) {
                Logger::info("WiFI connected");
            } else {
                Logger::error("WiFI connection is still not ready, will retry in 5s");
            }
            delay(5000);
        }
    }
}

void Connectivity::autoconnectToMqtt() {
    if (!Connectivity::client.connected()) {
        Logger::info("MQTT trying to connect");
        Connectivity::client.setServer(Configurations::data.MQTT_SERVER.c_str(), Configurations::data.MQTT_PORT);
        Connectivity::client.setCallback(callback);
        while (!Connectivity::client.connected()) {
            if (Connectivity::client.connect(Configurations::data.ID.c_str())) {
                Logger::info("MQTT connected");
                Connectivity::subscribe(Configurations::data.MQTT_TOPIC_STATUS, Connectivity::onMessageStatus);
                Connectivity::subscribe(Configurations::data.MQTT_TOPIC_DATA, Connectivity::onMessageData);
                Connectivity::subscribe(Configurations::data.MQTT_TOPIC_CONFIGURATION, Connectivity::callbackNoop);
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
            fullTopic = "cmnd/" + Connectivity::getTopic(topic);
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
    Connectivity::autoconnectToMqtt();
    Connectivity::sendDataTimer.tick();
    client.loop();
}

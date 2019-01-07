#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ethernet.h>

#include <connectivity.h>
#include <configurations.h>
#include <logger.h>

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

void Connectivity::connectToWifi() {
    WiFi.begin(Configurations::WIFI_SSID.c_str(), Configurations::WIFI_PASSWORD.c_str());
    Logger::log("Connection to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Logger::log("WiFi connection is still not ready");
        delay(1000);
    }
    Logger::log("Connected, IP address: " + WiFi.localIP());
}

void Connectivity::connectToMqtt() {
    Logger::log("Connecting to MQTT");
    client.setServer("192.168.0.2", 1883);
    client.setCallback(callback);
    while (!client.connected()) {
        if (client.connect(Configurations::ID.c_str())) {
            Logger::log("Connected to MQTT");
            sendStatus();
        } else {
            Logger::log("MQTT connection is still not ready; will retry in 5s");
        }
        delay(5000);
    }
}

void Connectivity::sendStatus() {
    
}

void Connectivity::loop() {
  client.loop();
}
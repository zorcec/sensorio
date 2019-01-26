#include <administration.h>
#include <configurations.h>
#include <connectivity.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

Timer<1> Administration::sleepTimer;

void Administration::initialize() {
    Logger::info("Initializing administration");
    Connectivity::subscribe(Configurations::data.MQTT_TOPIC_ADMINISTRATION, Administration::onMessage);

    // OTA
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        Logger::info("Start updating " + type);
    });

    ArduinoOTA.onEnd([]() {
        Logger::info("Updateing finished");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Logger::debug("-> " + String(progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Logger::error("Error[: " + error);
        if (error == OTA_AUTH_ERROR) {
            Logger::error("-> Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Logger::error("-> Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Logger::error("-> Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Logger::error("-> Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Logger::error("-> End Failed");
        }
    });
    
    ArduinoOTA.begin();

    // power saving
    wifi_set_sleep_type(LIGHT_SLEEP_T);
    if (Configurations::data.SLEEP_INTERVAL > 0) {
        Logger::debug("-> light sleep enabled");
        Logger::debug(" -> interval " + String(Configurations::data.SLEEP_INTERVAL));
        Logger::debug(" -> time " + String(Configurations::data.SLEEP_TIME));
        Administration::sleepTimer.every(
            Configurations::data.SLEEP_INTERVAL, 
            [](void*) -> bool { 
                Logger::trace("Entering sleep for " + String(Configurations::data.SLEEP_TIME));
                delay(Configurations::data.SLEEP_TIME);
                return true;
             }
        );      
    }
}

void Administration::onMessage(JsonObject& data) {
    String command;
    if (data.containsKey("command")) {
        command = data["command"].as<String>();
    }
    Logger::info("Administration received comand: " + command);
    if (command == "reset") {
        ESP.reset();
    }
    else if (command == "restart") {
        ESP.restart();
    }
}

void Administration::loop() {
    Administration::sleepTimer.tick();
    ArduinoOTA.handle();
}

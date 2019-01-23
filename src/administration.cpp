#include <administration.h>
#include <configurations.h>
#include <connectivity.h>
#include <Esp.h>

void Administration::initialize() {
    Logger::info("Initializing administration");
    Connectivity::subscribe(Configurations::data.MQTT_TOPIC_ADMINISTRATION, Administration::onMessage);
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

}

#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>

void initializationIsDone() {
  String message = "{NAME} [{VERSION}] is started";
  message.replace("{NAME}", Configurations::NAME);
  message.replace("{VERSION}", Configurations::VERSION);
  Logger::log(message);
}

void setup() {
  Logger::initialize();
  Connectivity::connectToWifi();
  Connectivity::connectToMqtt();
  initializationIsDone();
}

void loop() {
  // put your main code here, to run repeatedly:
  Connectivity::loop();
}

#include <Arduino.h>
#include <configurations.h>
#include <logger.h>
#include <connectivity.h>

void setup() {
  Logger::initialize();
  Connectivity::connectToWifi();
  Connectivity::connectToMqtt();
  Connectivity::sendStatus();
}

void loop() {
  // put your main code here, to run repeatedly:
  Connectivity::loop();
}

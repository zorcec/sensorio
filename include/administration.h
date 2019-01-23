#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Administration {
    public:
        static void initialize();
        static void loop();
        static void onMessage(JsonObject&);

};

#endif

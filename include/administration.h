/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <timer.h>

class Administration {
    public:
        static void initialize();
        static void loop();
        static void onMessage(JsonObject&);

    private:
        static Timer<1> sleepTimer;
};

#endif

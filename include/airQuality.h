#ifndef AIRQUALITY_H
#define AIRQUALITY_H

#include <Arduino.h>
#include <permanentStorage.h>

class AirQuality {

    public:
        static void initialize();
        static void loop();
        static float_t airQualityMin;
        static float_t airQualityMax;
        static float_t airQualityAdaptive;

};

#endif

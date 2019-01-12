#include <Arduino.h>
#include <sensors.h>

class SensorBME280 {
    public:
        static void initialize();
        static void loop(SensorsData*);

};

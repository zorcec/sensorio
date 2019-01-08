#include <Arduino.h>

class Sensors {
    public:
        static void initialize();
        static void loop();
        static void readBME280();
        static void startBME280();
        static float getTemperature();
        static float getHumidity();
        static float getPressure();
};

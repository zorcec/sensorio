#include <Arduino.h>

enum SensorTypes {
    TSL2561,
    BME280
};

struct SensorsData {
    float temperature;
    float pressure;
    float humidity;
};

class Sensors {
    public:
        static void initialize();
        static void loop();
        static void readBME280();
        static void startBME280();
        static SensorsData* getData();

    private:
        static void startI2c();
        static void scan(int*);
        static void start(int*);
        static bool isSensorActive(SensorTypes);

};

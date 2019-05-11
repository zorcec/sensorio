#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#define SUPORTED_SENSORS_NR 2

enum SensorTypes {
    LIGHT_TSL2561,
    AIR_BME280
};

enum SensorAnalogTypes {
    MQ135,
    CDS1
};

struct SensorsData {
    float_t temperature;
    float_t pressure;
    float_t humidity;
    uint16_t visibleLight;
    uint16_t infraredLight;
    uint16_t fullSpectrumLight;
    int8_t RSSI;
    float_t airQuality;
    float_t brightness;
    int16_t analogValue;
};

class Sensors {
    public:
        static void initialize();
        static void loop();
        static void readBME280();
        static void startBME280();
        static SensorsData data;

    private:
        static void startI2c();
        static void scan(int*);
        static void start(int*);
        static bool isSensorActive(SensorTypes);
        static int activeSensors[SUPORTED_SENSORS_NR];
        
};

#endif

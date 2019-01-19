#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorTSL2561.h>

#include "TSL2561.h"

TSL2561 sensorLightTSL2561(TSL2561_ADDR_FLOAT); 

Timer<1> SensorTSL2561::sensorReadTimer;

void SensorTSL2561::initialize() {
    Logger::info("Initializing TSL2561");
    if (!sensorLightTSL2561.begin()) {
        Logger::warn("-> not responding");
    } else {
        Logger::info("-> Sensor found");
    }

    // configuration
    sensorLightTSL2561.setGain(TSL2561_GAIN_16X);                 // set 16x gain (for dim situations)
    sensorLightTSL2561.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)

    SensorTSL2561::refresh();
    SensorTSL2561::sensorReadTimer.every(
        Configurations::REFRESH_INTERVALS.TSL2561, 
        [](void*) -> bool { return SensorTSL2561::refresh(); }
    );
};

bool SensorTSL2561::refresh() {
    Logger::trace("Refreshing TSL2561");
    Sensors::data.visibleLight = sensorLightTSL2561.getLuminosity(TSL2561_VISIBLE);     
    Sensors::data.infraredLight = sensorLightTSL2561.getLuminosity(TSL2561_INFRARED);
    Sensors::data.fullSpectrumLight = sensorLightTSL2561.getLuminosity(TSL2561_FULLSPECTRUM);
    Logger::trace("-> visible light:\t" + String(Sensors::data.visibleLight));
    Logger::trace("-> infrared light:\t" + String(Sensors::data.infraredLight));
    Logger::trace("-> full spectrum:\t" + String(Sensors::data.fullSpectrumLight));
    return true;
};

void SensorTSL2561::loop() {
    SensorTSL2561::sensorReadTimer.tick();
}

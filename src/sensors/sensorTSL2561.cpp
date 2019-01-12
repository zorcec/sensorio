#include <Arduino.h>

#include <logger.h>
#include <configurations.h>
#include <sensors/sensorTSL2561.h>

#include "TSL2561.h"

TSL2561 sensorLightTSL2561(TSL2561_ADDR_FLOAT); 

void SensorTSL2561::initialize() {
    Logger::info("Initializing TSL2561");
    if (!sensorLightTSL2561.begin()) {
        Logger::warn("-> not responding");
    }

    // configuration
    sensorLightTSL2561.setGain(TSL2561_GAIN_16X);                 // set 16x gain (for dim situations)
    sensorLightTSL2561.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // medium integration time (medium light)
};

void SensorTSL2561::loop(SensorsData* data) {
    Logger::info("Refreshing TSL2561");
    data->visibleLight = sensorLightTSL2561.getLuminosity(TSL2561_VISIBLE);     
    data->infraredLight = sensorLightTSL2561.getLuminosity(TSL2561_INFRARED);
    data->fullSpectrumLight = sensorLightTSL2561.getLuminosity(TSL2561_FULLSPECTRUM);
    Logger::debug("-> visible light:\t" + String(data->visibleLight));
    Logger::debug("-> infrared light:\t" + String(data->infraredLight));
    Logger::debug("-> full spectrum:\t" + String(data->fullSpectrumLight));
};

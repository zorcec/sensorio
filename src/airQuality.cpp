#include <airQuality.h>
#include <configurations.h>
#include <logger.h>

float_t AirQuality::airQualityMax = 0;
float_t AirQuality::airQualityMin = 255;
float_t AirQuality::airQualityAdaptive = 0;

void AirQuality::initialize() {
    Logger::info("Initializing air quality");
    Logger::info("-> starting with air quality min: " + String(Configurations::data.AIR_QUALITY_MIN));
    Logger::info("-> starting with air quality max: " + String(Configurations::data.AIR_QUALITY_MAX));
    AirQuality::airQualityMin = Configurations::data.AIR_QUALITY_MIN;
    AirQuality::airQualityMax = Configurations::data.AIR_QUALITY_MAX;
}

void AirQuality::loop() {
    if(Sensors::data.airQuality < AirQuality::airQualityMin) {
        AirQuality::airQualityMin = Sensors::data.airQuality;
    }
    if(Sensors::data.airQuality > AirQuality::airQualityMax) {
        AirQuality::airQualityMax = Sensors::data.airQuality;
    }
    AirQuality::airQualityAdaptive = (Sensors::data.airQuality - AirQuality::airQualityMin) / (AirQuality::airQualityMax - AirQuality::airQualityMin);
}
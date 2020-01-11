/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <sensors.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class SensorDS18B20 {

    public:
        static void initialize();
        static void loop();
        static bool refresh();

    private:
        static bool isActive;
        static OneWire oneWire;
        static DallasTemperature sensor;

};

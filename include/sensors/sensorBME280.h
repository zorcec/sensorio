/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <sensors.h>
#include <timer.h>

class SensorBME280  {
    public:
        static void initialize();
        static void loop();
        static bool refresh();

    private:
        static bool isActive;
};

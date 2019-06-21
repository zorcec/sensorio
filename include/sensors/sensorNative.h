/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <sensors.h>

class SensorNative {
    public:
        static void initialize();
        static void loop();
        static bool refresh();

    private:
        static bool isActive;

};

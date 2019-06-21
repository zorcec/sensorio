/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <sensors.h>

class SensorCDS1 {

    public:
        static void initialize();
        static void loop();
        static bool refresh();

    private:
        static bool isActive;

};

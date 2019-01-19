#include <sensors.h>
#include <timer.h>

class SensorNative {
    public:
        static void initialize();
        static void loop();

    private:
        static bool refresh();
        static Timer<1> sensorReadTimer;

};

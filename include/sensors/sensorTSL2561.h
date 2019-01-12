#include <sensors.h>
#include <timer.h>

class SensorTSL2561 {
    public:
        static void initialize();
        static void loop();

    private:
        static bool refresh(void *);
        static Timer<1> sensorReadTimer;

};

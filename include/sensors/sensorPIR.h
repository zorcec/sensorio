#include <sensors.h>
#include <timer.h>

class SensorPIR {
    public:
        static void initialize();
        static void loop();

    private:
        static void interupt();

};

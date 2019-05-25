#include <sensors.h>

class SensorPIR {
    public:
        static void initialize();
        static void loop();
        static void interupt();

    private:
        static bool isActive;

};

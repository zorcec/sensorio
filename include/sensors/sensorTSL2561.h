#include <sensors.h>

class SensorTSL2561 {
    public:
        static void initialize();
        static void loop();
        static bool refresh();

    private:
        static bool isActive;

};

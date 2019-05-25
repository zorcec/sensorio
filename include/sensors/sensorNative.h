#include <sensors.h>
#include <timer.h>

class SensorNative {
    public:
        static void initialize();
        static void loop();
        static bool refresh();

    private:
        static bool isActive;

};

#include <Arduino.h>

class Logger {
    public:
        static void initialize();
        static void log(String);

    private:
        static bool isSerialReady;
};

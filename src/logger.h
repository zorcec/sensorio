#include <Arduino.h>

class Logger {
    public:
        static void initialize();
        static void log(String message);

    private:
        static bool isSerialReady;
};

#include <Arduino.h>

enum LogType {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
    public:
        static void initialize();
        static void log(String);
        static void log(LogType, String);
        static void debug(String);
        static void info(String);
        static void warn(String);
        static void error(String);

    private:
        static bool isSerialReady;
};

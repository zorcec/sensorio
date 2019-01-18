#include <Arduino.h>
#include <configurations.h>
#include <logger.h>

enum LedColor {
    RED,
    GREEN,
    BLUE
};

enum LedEffect {
    NORMAL,
    BLINK_SLOW,
    BLINK_FAST
};

class Notifications {
    public:
        static void initialize();
        static void led(LedColor, float_t, LedEffect);
    
    private:
        static int8_t calculateBrightness(int8_t);
};

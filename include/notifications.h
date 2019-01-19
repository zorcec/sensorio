#include <Arduino.h>
#include <configurations.h>
#include <logger.h>

enum LedColor {
    RED,
    GREEN,
    BLUE
};

enum LedEffect {
    BLINK_1X,
    BLINK_3X,
    CONTINUOUS_BLINK,
    FADE
};

class Notifications {
    public:
        static void initialize();
        static void loop();
        static void led(LedColor, float_t);
        static void led(LedColor, float_t, LedEffect*);
    
    private:
        static int16_t calculateBrightness(int8_t);
        static String subscribeTopicNotification;
};

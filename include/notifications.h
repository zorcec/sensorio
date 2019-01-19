#include <Arduino.h>
#include <ArduinoJson.h>
#include <configurations.h>
#include <logger.h>

enum LedColor {
    RED,
    GREEN,
    BLUE,
    PURPLE,
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
        static void onMessage(JsonObject&);
    
    private:
        static float_t calculateBrightness(float_t);
        static String subscribeTopicNotification;
        static void changeLed(int8_t, float_t);
};

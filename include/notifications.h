#include <Arduino.h>
#include <ArduinoJson.h>
#include <configurations.h>
#include <logger.h>
#include <timer.h>

enum LedColor {
    RED,
    GREEN,
    BLUE,
    PURPLE,
    ORANGE,
    TURQUOISE,
    WHITE
};

enum LedEffect {
    BLINK_1X,
    BLINK_3X,
    CONTINUOUS_BLINK
};

class Notifications {
    public:
        static void initialize();
        static void loop();
        static void led(LedColor, float_t);
        static void led(LedColor, float_t, LedEffect*);
        static void onMessage(JsonObject&);
        static LedColor currentColor;
        static float_t currentBrightness;
        static LedEffect* currentEffect;
    
    private:
        static float_t calculateBrightness(float_t);
        static String subscribeTopicNotification;
        static void changeLed(int8_t, float_t);
        
};

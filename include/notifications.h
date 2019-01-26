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
    NONE,
    BLINK
};

class Notifications {
    public:
        static void initialize();
        static void loop();
        static void led(LedColor, float_t);
        static void led(LedColor, float_t, LedEffect);
        static void onMessage(JsonObject&);
        static float_t currentPwmRed;
        static float_t currentPwmGreen;
        static float_t currentPwmBlue;
        static bool ledPowerState;
        static int16_t elapsedBlinkingTime;
        static LedEffect currentEffect;
    
    private:
        static float_t calculateBrightness(float_t);
        static String subscribeTopicNotification;
        static void changeLed(int8_t, float_t);
        static Timer<1> effectBlinkTimer;
        
};

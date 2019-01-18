#include <notifications.h>

#define PWM_MAX 1023

void Notifications::initialize() {
    Logger::info("Initializing Notifications");
    if (Configurations::LED_GPIO.RED > 0) {
        Logger::debug("-> LED (red) GPIO" + String(Configurations::LED_GPIO.RED));
        pinMode(Configurations::LED_GPIO.RED, OUTPUT);
        analogWrite(Configurations::LED_GPIO.RED, PWM_MAX);
    }
    if (Configurations::LED_GPIO.GREEN > 0) {
        Logger::debug("-> LED (green) GPIO" + String(Configurations::LED_GPIO.GREEN));
        pinMode(Configurations::LED_GPIO.GREEN, OUTPUT);
        analogWrite(Configurations::LED_GPIO.GREEN, PWM_MAX);
    }
    if (Configurations::LED_GPIO.BLUE > 0) {
        Logger::debug("-> LED (blue) GPIO" + String(Configurations::LED_GPIO.BLUE));
        pinMode(Configurations::LED_GPIO.BLUE, OUTPUT);
        analogWrite(Configurations::LED_GPIO.BLUE, PWM_MAX);
    }
};

int8_t Notifications::calculateBrightness(int8_t percentage) {
    return PWM_MAX / percentage;
};

void Notifications::led(LedColor color, float_t brightness, LedEffect effect) {

};

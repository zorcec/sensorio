#include <notifications.h>
#include <connectivity.h>

#define PWM_MAX 1023

void Notifications::initialize() {
    Logger::info("Initializing Notifications");
    if (Configurations::LED_GPIO.RED > 0) {
        Logger::debug("-> LED (red) GPIO" + String(Configurations::LED_GPIO.RED));
        pinMode(Configurations::LED_GPIO.RED, OUTPUT);
        Notifications::led(LedColor::RED, 0);
    }
    if (Configurations::LED_GPIO.GREEN > 0) {
        Logger::debug("-> LED (green) GPIO" + String(Configurations::LED_GPIO.GREEN));
        pinMode(Configurations::LED_GPIO.GREEN, OUTPUT);
        Notifications::led(LedColor::GREEN, 0);
    }
    if (Configurations::LED_GPIO.BLUE > 0) {
        Logger::debug("-> LED (blue) GPIO" + String(Configurations::LED_GPIO.BLUE));
        pinMode(Configurations::LED_GPIO.BLUE, OUTPUT);
        Notifications::led(LedColor::BLUE, 0);
    }
    Connectivity::subscribe(Configurations::MQTT_TOPIC_NOTIFY, false);
};

int16_t Notifications::calculateBrightness(int8_t percentage) {
    if (percentage <= 0) {
        return 0;
    }
    return PWM_MAX * (percentage / 100);
};

void Notifications::led(LedColor color, float_t brightness) {
    LedEffect effects[] = { LedEffect::FADE };
    Notifications::led(color, brightness, effects);
}

void Notifications::led(LedColor color, float_t brightness, LedEffect* effect) {
    int8_t gpio = -1;
    if (color == LedColor::RED) {
        gpio = Configurations::LED_GPIO.RED;
    } else if (color == LedColor::GREEN) {
        gpio = Configurations::LED_GPIO.GREEN;
    } else if (color == LedColor::BLUE) {
        gpio = Configurations::LED_GPIO.BLUE;
    }
    if (gpio >= 0) {
        Logger::debug("Changing led state on GPIO " + String(gpio) + ", color: " + String(color) + ", brightness: " + String(brightness));
        analogWrite(gpio, PWM_MAX);
    }
};

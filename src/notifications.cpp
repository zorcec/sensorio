#include <notifications.h>
#include <connectivity.h>

#define PWM_MAX 1023.0f

LedColor Notifications::currentColor = LedColor::RED;
float_t Notifications::currentBrightness = 0;
LedEffect* Notifications::currentEffect = {};

void Notifications::initialize() {
    Logger::info("Initializing Notifications");
    if (Configurations::data.LED_GPIO.RED > 0) {
        Logger::debug("-> LED (red) GPIO" + String(Configurations::data.LED_GPIO.RED));
        pinMode(Configurations::data.LED_GPIO.RED, OUTPUT);
        Notifications::led(LedColor::RED, 0);
    }
    if (Configurations::data.LED_GPIO.GREEN > 0) {
        Logger::debug("-> LED (green) GPIO" + String(Configurations::data.LED_GPIO.GREEN));
        pinMode(Configurations::data.LED_GPIO.GREEN, OUTPUT);
        Notifications::led(LedColor::GREEN, 0);
    }
    if (Configurations::data.LED_GPIO.BLUE > 0) {
        Logger::debug("-> LED (blue) GPIO" + String(Configurations::data.LED_GPIO.BLUE));
        pinMode(Configurations::data.LED_GPIO.BLUE, OUTPUT);
        Notifications::led(LedColor::BLUE, 0);
    }
    Connectivity::subscribe(Configurations::data.MQTT_TOPIC_NOTIFY, Notifications::onMessage);
};

void Notifications::onMessage(JsonObject& data) {
    Logger::info("Notification received message");
    LedColor color;
    uint8_t brightness = 100;
    if (data.containsKey("gpio") && data.containsKey("pwm")) {
        uint8_t gpio = data["gpio"].as<uint8_t>();
        uint16_t pwm = data["pwm"].as<uint16_t>();
        pinMode(gpio, OUTPUT);
        analogWrite(gpio, pwm);
        Logger::info("Testing GPIO" + String(gpio) + ", pwm: " + String(pwm));
    }
    if (data.containsKey("color")) {
        color = static_cast<LedColor>(data["color"].as<uint8_t>());
    }
    if (data.containsKey("brightness")) {
        brightness = data["brightness"].as<uint8_t>();
    }
    Notifications::led(color, brightness);
}


float_t Notifications::calculateBrightness(float_t percentage) {
    if (percentage <= 0) {
        return PWM_MAX;
    }
    return PWM_MAX - PWM_MAX * (percentage / 100.0f);
};

void Notifications::led(LedColor color, float_t brightness) {
    LedEffect effects[] = { };
    Notifications::led(color, brightness, effects);
}

void Notifications::led(LedColor color, float_t brightness, LedEffect* effect) {
    float_t pwmValue = Notifications::calculateBrightness(brightness);
    Notifications::currentColor = color;;
    Notifications::currentBrightness = brightness;
    Notifications::currentEffect = effect;
    if (Configurations::data.LED_GPIO.RED >= 0) {
        analogWrite(Configurations::data.LED_GPIO.RED, PWM_MAX);
    }
    if (Configurations::data.LED_GPIO.GREEN >= 0) {
        analogWrite(Configurations::data.LED_GPIO.GREEN, PWM_MAX);
    }
    if (Configurations::data.LED_GPIO.BLUE >= 0) {
        analogWrite(Configurations::data.LED_GPIO.BLUE, PWM_MAX);
    }
    switch(color) {
        case LedColor::RED:
            Notifications::changeLed(Configurations::data.LED_GPIO.RED, pwmValue);
            break;
        case LedColor::GREEN:
            Notifications::changeLed(Configurations::data.LED_GPIO.GREEN, pwmValue);
            break;
        case LedColor::BLUE:
            Notifications::changeLed(Configurations::data.LED_GPIO.BLUE, pwmValue);
            break;
        case LedColor::PURPLE:
            Notifications::changeLed(Configurations::data.LED_GPIO.RED, pwmValue);
            Notifications::changeLed(Configurations::data.LED_GPIO.BLUE, pwmValue);
            break;
        case LedColor::ORANGE:
            Notifications::changeLed(Configurations::data.LED_GPIO.RED, pwmValue);
            Notifications::changeLed(Configurations::data.LED_GPIO.GREEN, pwmValue);
            break;
        case LedColor::TURQUOISE:
            Notifications::changeLed(Configurations::data.LED_GPIO.BLUE, pwmValue);
            Notifications::changeLed(Configurations::data.LED_GPIO.GREEN, pwmValue);
            break;
        case LedColor::WHITE:
            Notifications::changeLed(Configurations::data.LED_GPIO.RED, pwmValue);
            Notifications::changeLed(Configurations::data.LED_GPIO.BLUE, pwmValue);
            Notifications::changeLed(Configurations::data.LED_GPIO.GREEN, pwmValue);
            break;
    }
};

void Notifications::changeLed(int8_t gpio, float_t pwmValue) {
    if (gpio >= 0) {
        Logger::debug("Changing led state on GPIO " + String(gpio) + ", pwm: " + String(pwmValue));
        analogWrite(gpio, pwmValue);
    }
}

void Notifications::loop() {
    
};

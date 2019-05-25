#include <notifications.h>
#include <connectivity.h>

#define PWM_MAX 1023.0f
#define BLINK_INTERVAL 500

float_t Notifications::currentPwmRed;
float_t Notifications::currentPwmGreen;
float_t Notifications::currentPwmBlue;
int16_t Notifications::elapsedBlinkingTime;
bool Notifications::ledPowerState;
LedEffect Notifications::currentEffect;
Timer<1> Notifications::effectBlinkTimer;

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
    Connectivity::subscribe(Configurations::data.MQTT_TOPIC_NOTIFICATIONS, Notifications::onMessage);
};

void Notifications::onMessage(JsonObject& data) {
    Logger::info("Notification received message");
    LedColor color = LedColor::WHITE;
    LedEffect effect = LedEffect::NONE;
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
    if (data.containsKey("effect")) {
        effect = static_cast<LedEffect>(data["effect"].as<uint8_t>());
    }
    Notifications::led(color, brightness, effect);
}


float_t Notifications::calculateBrightness(float_t percentage) {
    if (percentage <= 0) {
        return PWM_MAX;
    }
    return PWM_MAX - PWM_MAX * (percentage / 100.0f);
}

void Notifications::led(LedColor color, float_t brightness) {
    Notifications::led(color, brightness, LedEffect::NONE);
}

void Notifications::led(LedColor color, float_t brightness, LedEffect effect) {
    float_t pwmValue = Notifications::calculateBrightness(brightness);
    Notifications::elapsedBlinkingTime = 0;
    Notifications::ledPowerState = false;
    Notifications::currentEffect = effect;
    currentPwmRed = PWM_MAX;
    currentPwmBlue = PWM_MAX;
    currentPwmGreen = PWM_MAX;
    switch(color) {
        case LedColor::RED:
            currentPwmRed = pwmValue;
            break;
        case LedColor::GREEN:
            currentPwmGreen = pwmValue;
            break;
        case LedColor::BLUE:
            currentPwmBlue = pwmValue;
            break;
        case LedColor::PURPLE:
            currentPwmRed = pwmValue;
            currentPwmBlue = pwmValue;
            break;
        case LedColor::ORANGE:
            currentPwmRed = pwmValue;
            currentPwmGreen = pwmValue;
            break;
        case LedColor::TURQUOISE:
            currentPwmBlue= pwmValue;
            currentPwmGreen = pwmValue;
            break;
        case LedColor::WHITE:
            currentPwmRed = pwmValue;
            currentPwmBlue = pwmValue;
            currentPwmGreen = pwmValue;
            break;
    }

    if (effect != LedEffect::NONE) {
        Logger::debug("Activating effect");
        Notifications::effectBlinkTimer.every(
            BLINK_INTERVAL, [](void*) -> bool { 
                Notifications::elapsedBlinkingTime += BLINK_INTERVAL;
                Notifications::ledPowerState = !Notifications::ledPowerState;
                Notifications::changeLed(Configurations::data.LED_GPIO.RED, Notifications::ledPowerState ? Notifications::currentPwmRed : PWM_MAX);
                Notifications::changeLed(Configurations::data.LED_GPIO.GREEN, Notifications::ledPowerState ? Notifications::currentPwmGreen : PWM_MAX);
                Notifications::changeLed(Configurations::data.LED_GPIO.BLUE, Notifications::ledPowerState ? Notifications::currentPwmBlue : PWM_MAX);
                bool stopBlinking = Notifications::elapsedBlinkingTime >= Configurations::data.NOTIFICATIONS_LED_BLINK_TIME && !Notifications::ledPowerState;
                return !stopBlinking;
            }
        ); 
    } else {
        Notifications::changeLed(Configurations::data.LED_GPIO.RED, Notifications::currentPwmRed);
        Notifications::changeLed(Configurations::data.LED_GPIO.GREEN, Notifications::currentPwmGreen);
        Notifications::changeLed(Configurations::data.LED_GPIO.BLUE, Notifications::currentPwmBlue);
    }
}

void Notifications::changeLed(int8_t gpio, float_t pwmValue) {
    if (gpio >= 0) {
        Logger::trace("Changing led state on GPIO " + String(gpio) + ", pwm: " + String(pwmValue));
        analogWrite(gpio, pwmValue);
    }
}

void Notifications::loop() {
    Notifications::effectBlinkTimer.tick();
}

bool Notifications::getCurrentPowerState() {
    return Notifications::currentPwmRed < PWM_MAX || Notifications::currentPwmGreen < PWM_MAX || Notifications::currentPwmBlue < PWM_MAX;
}

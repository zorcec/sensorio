/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <permanentStorage.h>
#include <configurations.h>
#include <logger.h>

void PermanentStorage::initialize() {
    Logger::info("Initializing permanent storage");
    EEPROM.begin(512);
}

bool PermanentStorage::set(StorageDataMarker marker, int8_t value, int8_t guard) {
    Logger::debug("Trying to write a new value: " + String(value) + ", on marker: " + String(marker));
    int8_t savedValue = PermanentStorage::get(marker, 0);
    if (savedValue == 0 || abs(value - savedValue) > guard) {
        EEPROM.write(marker, value);
        bool result = EEPROM.commit();
        Logger::debug("-> " + result ? "OK" : "FAILED");
        return result;
    } else {
        Logger::trace("-> GUARDED");
    }
    return false;
}

int8_t PermanentStorage::get(StorageDataMarker marker, int8_t defaultValue) {
    int8_t value = EEPROM.read(marker);
    if(value == 0) {
        return defaultValue;
    }
    return value;
}

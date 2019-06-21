/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#ifndef PERMANENTSTORAGE_H
#define PERMANENTSTORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

enum StorageDataMarker {
    ALL_TIME_BEST_AIR_QUALITY = 0,
};

class PermanentStorage {

    public:
        static void initialize();
        static bool set(StorageDataMarker, int8_t, int8_t);
        static int8_t get(StorageDataMarker, int8_t);

};

#endif

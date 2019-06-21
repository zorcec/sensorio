#ifndef PASSIVE_TRACKING_H
#define PASSIVE_TRACKING_H

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <vector>
#include <timer.h>

#include <Arduino.h>

class PassiveTracking {

    public:
        static void initialize();
        static bool sendProbes(void *);
        static void loop();

    private:
        static WiFiEventHandler probeRequestPrintHandler;
        static String macToString(const unsigned char*);
        static std::vector<WiFiEventSoftAPModeProbeRequestReceived> receivedProbeRequests;
        static Timer<1> sendProbesTimer;
        static void onProbeRequest(const WiFiEventSoftAPModeProbeRequestReceived&);

};

#endif

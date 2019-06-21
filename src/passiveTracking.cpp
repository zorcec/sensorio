/*
 * Author:  Tomislav Zorčec
 * URL:     https://github.com/zorcec/sensorio
 */

#include <passiveTracking.h>
#include <configurations.h>
#include <connectivity.h>

WiFiEventHandler PassiveTracking::probeRequestPrintHandler;
std::vector<WiFiEventSoftAPModeProbeRequestReceived> PassiveTracking::receivedProbeRequests;
Timer<1> PassiveTracking::sendProbesTimer;

void PassiveTracking::initialize() {
    Logger::info("Initializing passive tracking");
    WiFi.persistent(false);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(Configurations::data.ID, Configurations::data.NAME);
    PassiveTracking::probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&PassiveTracking::onProbeRequest);
    PassiveTracking::sendProbesTimer.every(Configurations::data.SEND_PROBES_INTERVAL, PassiveTracking::sendProbes);
}

void ICACHE_RAM_ATTR PassiveTracking::onProbeRequest(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  PassiveTracking::receivedProbeRequests.push_back(evt);
  Logger::debug("Revceived probe request: " + macToString(evt.mac) + ", RSSI: " + evt.rssi);
}

bool PassiveTracking::sendProbes(void *) {
    if (PassiveTracking::receivedProbeRequests.size() > 0) {
        Logger::debug("Sending probes");
        DynamicJsonBuffer jsonBuffer;
        JsonObject& data = jsonBuffer.createObject();
        JsonArray& probes = data.createNestedArray("probes");
        for(WiFiEventSoftAPModeProbeRequestReceived w : PassiveTracking::receivedProbeRequests){
            JsonObject& probe = probes.createNestedObject();
            probe["address"] = macToString(w.mac);
            probe["rssi"] = w.rssi;
        }
        Logger::debug("-> sending MQTT message");
        Connectivity::sendJson("PROBES", data);
        if (Configurations::data.FIND_SERVER.length() > 0) {
            PassiveTracking::sendToFind3();
        }
        PassiveTracking::receivedProbeRequests.clear();
    }
    return true;
}

void PassiveTracking::sendToFind3() {
    Logger::debug("-> posting to FIND3");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();
    data["d"] = Configurations::data.ID;                // device
    data["f"] = Configurations::data.FIND_FAMILY;       // family
    data["s"] = jsonBuffer.createObject();              // sensors / probes
    JsonArray& probes = data.createNestedArray("probes");
    for(WiFiEventSoftAPModeProbeRequestReceived w : PassiveTracking::receivedProbeRequests){
        JsonObject& probe = probes.createNestedObject();
        probe[macToString(w.mac)] = w.rssi;
    }
    data["s"]["wifi"] = probes;

    // passive scanning
    Connectivity::sendJsonHttp(Configurations::data.FIND_SERVER + "/passive", data);
    
    // general scanning / learning
    if (false) {
        data["l"] = Configurations::data.FIND_LOCATION;     // location; if set will learn
        Connectivity::sendJsonHttp(Configurations::data.FIND_SERVER + "/data", data);
    }
}

String PassiveTracking::macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void PassiveTracking::loop() {
    PassiveTracking::sendProbesTimer.tick();
}

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
    WiFi.softAP(Configurations::data.NAME, Configurations::data.NAME);
    PassiveTracking::probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&PassiveTracking::onProbeRequest);
    PassiveTracking::sendProbesTimer.every(Configurations::data.SEND_PROBES_INTERVAL, PassiveTracking::sendProbes);
}

void ICACHE_RAM_ATTR PassiveTracking::onProbeRequest(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
  PassiveTracking::receivedProbeRequests.push_back(evt);
  Logger::debug("Revceived probe request: " + macToString(evt.mac) + ", RSSI: " + evt.rssi);
}

bool PassiveTracking::sendProbes(void *) {
    if (PassiveTracking::receivedProbeRequests.size() > 0) {
        DynamicJsonBuffer jsonBuffer;
        JsonObject& data = jsonBuffer.createObject();
        JsonArray& probes = data.createNestedArray("probes");
        for(WiFiEventSoftAPModeProbeRequestReceived w : PassiveTracking::receivedProbeRequests){
            JsonObject& probe = probes.createNestedObject();
            probe["address"] = macToString(w.mac);
            probe["rssi"] = w.rssi;
        }
        PassiveTracking::receivedProbeRequests.clear();
        Connectivity::sendJson("PROBES", data);
        Logger::debug("Sending probes");
    }
    return true;
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

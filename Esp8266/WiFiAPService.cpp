#include "WiFiAPService.h"

WiFiAPService::WiFiAPService() {}

WiFiAPService::~WiFiAPService() {
  stop();
}

bool WiFiAPService::isRunning() {
  return _running;
}

bool WiFiAPService::start() {

  if (!isRunning()) {
    // general settings
    WiFi.enableAP(true);
    WiFi.persistent(false);
    WiFi.setAutoConnect(true);

    /**
    connectedEventHandler = WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected& event) {
      Serial.println(">>> connected");
    });
    disconnectedEventHandler = WiFi.onSoftAPModeStationDisconnected([](const WiFiEventSoftAPModeStationDisconnected& event) {
      Serial.println(">>> disconnected");
    });
    */

    // try to establish a soft AP
    if (WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWD, WIFI_AP_CHANNEL, WIFI_AP_HIDDEN)) {
      Log.notice(F("Soft AP established successful. IP address of AP is: %s" CR), WiFi.softAPIP().toString().c_str());
      _running = true;
    } else {
      Log.error(F("Couldn't establish a soft access point." CR));
    }
    // try to enable multicast DNS if HOST_NAME is given
    #ifdef HOST_NAME
      setupMDNS();
    #endif
  }

  return isRunning();
}

bool WiFiAPService::stop() {

  if (isRunning()) {
    WiFi.softAPdisconnect();
    _running = false;
  }

  return isRunning();
}

bool WiFiAPService::setupMDNS() {

  bool succeeded = false;
  // add <domain name>.local (mDNS)
  if (MDNS.begin(HOST_NAME)) {
    // add service
    MDNS.addService("http", "tcp", 80);
    succeeded = true;
  }

  if (succeeded) {
    Log.notice(F("MDNS established for [http://%s.local]" CR), HOST_NAME);
  } else {
    Log.error(F("MDNS failed for [http://%s.local]" CR), HOST_NAME);
  }

  return succeeded;
}

uint8_t WiFiAPService::getStationNumber() {
  return WiFi.softAPgetStationNum();
}

softap_config* WiFiAPService::getConfig() {

  softap_config *config;
  if (wifi_softap_get_config(config)) {
    Log.verbose(F("Reading soft-AP configuration was successful." CR));
  } else {
    Log.verbose(F("Reading soft-AP configuration failed." CR));
  }
  
  return config;
}



#include "WiFiAPService.h"

WiFiAPService::WiFiAPService() {
}

WiFiAPService::~WiFiAPService() {
  stop();
}

bool WiFiAPService::isRunning() {
  
  // TODO see return WiFi.isConnected();
  return running;
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
    if (WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWD, WIFI_AP_CHANNEL, WIFI_AP_HIDDEN, WIFI_AP_MAX_CONNECTIONS)) {
      Log.notice(F("Soft AP established successful. IP address of AP is: %s" CR), WiFi.softAPIP().toString().c_str());
      running = true;
    } else {
      Log.error(F("Couldn't establish a soft access point." CR));
    }
  }

  return isRunning();
}

bool WiFiAPService::stop() {

  if (isRunning()) {
    WiFi.softAPdisconnect();
    running = false;
  }

  return isRunning();
}

bool WiFiAPService::setupMDNS(const char* hostName, unsigned int port) {

  bool succeeded = false;
  // add <domain name>.local (mDNS)
  if (MDNS.begin(hostName)) {
    // add service
    MDNS.addService("http", "tcp", port);
    succeeded = true;
  }

  if (succeeded) {
    Log.notice(F("MDNS established for [http://%s.local]" CR), hostName);
  } else {
    Log.error(F("MDNS failed for [http://%s.local]" CR), hostName);
  }

  return succeeded;
}

ESP8266WiFiClass* WiFiAPService::getWiFi() {
  return &WiFi;
}

// TODO needed ?
softap_config* WiFiAPService::getConfig() {

  softap_config *config;
  if (wifi_softap_get_config(config)) {
    Log.verbose(F("Reading soft-AP configuration was successful." CR));
  } else {
    Log.verbose(F("Reading soft-AP configuration failed." CR));
  }
  
  return config;
}



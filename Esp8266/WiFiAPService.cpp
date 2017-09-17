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

JsonObject& WiFiAPService::getSoftAPDetails() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  json[F("softAPgetStationNum")] = getWiFi()->softAPgetStationNum();
  json[F("softAPIP")] = getWiFi()->softAPIP().toString();
  json[F("softAPmacAddress")] = getWiFi()->softAPmacAddress();
  
  softap_config *config;
  if (wifi_softap_get_config(config)) {
    Log.verbose(F("Reading soft-AP configuration was successful." CR));

    json[F("ssid")] = config->ssid;
    json[F("password")] = config->password;
    json[F("ssidLen")] = config->ssid_len;
    json[F("channel")] = config->channel;
    json[F("authmode")] = config->authmode;
    json[F("ssidHidden")] = config->ssid_hidden;
    json[F("maxConnections")] = config->max_connection;
    json[F("beaconInterval")] = config->beacon_interval;
       
  } else {
    Log.verbose(F("Reading soft-AP configuration failed." CR));
  }

  return json;
}



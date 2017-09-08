#include "WiFiStaService.h"

WiFiStaService::WiFiStaService() {}

WiFiStaService::~WiFiStaService() {
  stop();
}

bool WiFiStaService::isRunning() {
  return WiFi.isConnected();
}

bool WiFiStaService::start() {

  uint8_t retries = 20;
  if (!isRunning()) {
    // general settings
    WiFi.enableSTA(true);
    WiFi.persistent(false);
    WiFi.setAutoConnect(true);

    /**
      connectedEventHandler = WiFi.onStationModeConnected([](const WiFiEventStationModeConnected& event) {
        Log.verbose(F("Station connected"));
      });
      disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event) {
        Log.verbose(F("Station disconnected"));
      });
      authModeChangedEventHandler = WiFi.onStationModeAuthModeChanged([](const WiFiEventStationModeAuthModeChanged & event) {
        Log.verbose(F("Station auth mode changed"));
      });
      gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event) {
        Log.verbose(F("Station connected, IP: %s" CR), WiFi.localIP().toString().c_str());
      });
    */

    // try to connect
    // TODO implement an "auto-detect" of all available WiFi credentials
    wifiMulti.addAP(WIFI_SSID_1, WIFI_PASSWD_1);
    wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWD_2);
    Log.verbose("Trying to connect WiFi ");
    while (wifiMulti.run() != WL_CONNECTED && retries-- > 0) { // try to connect at least 20 times
      delay(300); 
      if (LOG_LEVEL == LOG_LEVEL_VERBOSE) {
        Serial.print(F("."));
      }
    }
    Serial.println();
  }
  if (retries == 0) {
    Log.error(F("Failed to setup a WiFi connection. Please check your WiFi availability / accessibility and restart." CR));
    stop(); // => stop service
  } else {
    Log.notice(F("WiFi successful connected. IP address is: %s" CR), WiFi.localIP().toString().c_str());
  }

  return isRunning();
}

bool WiFiStaService::stop() {

  if (isRunning()) {
    WiFi.disconnect();
  }

  return isRunning();
}

IPAddress WiFiStaService::getLocalIp() {
  return WiFi.localIP();
}


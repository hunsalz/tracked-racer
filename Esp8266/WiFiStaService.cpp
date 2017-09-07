#include "WiFiStaService.h"

WiFiStaService::WiFiStaService() {}

WiFiStaService::~WiFiStaService() {
  stop();
}

bool WiFiStaService::isRunning() {
  return WiFi.isConnected();
}

bool WiFiStaService::start() {

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
    uint8_t i = 0;
    while (wifiMulti.run() != WL_CONNECTED && i++ < 20) { // try to connect at least 20 times
      delay(300); 
      if (LOG_LEVEL == LOG_LEVEL_VERBOSE) {
        Serial.print(F("."));
      }
    }
    Serial.println();
    if (i > 20) {
      Log.error(F("Couldn't connect to any WiFi AP. Please check your WiFi availability / accessibility and restart." CR));
    } else {
      Log.notice(F("WiFi successful connected. IP address is: %s" CR), WiFi.localIP().toString().c_str());
    }
  }

  return isRunning();
}

bool WiFiStaService::stop() {

  if (isRunning()) {
    WiFi.disconnect();
  }

  return isRunning();
}

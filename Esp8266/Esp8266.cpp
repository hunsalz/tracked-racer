#include "Esp8266.h"

Esp8266::Esp8266() : 
  shield(true, MOTOR_A_SPEED, MOTOR_A_DIR, MOTOR_B_SPEED, MOTOR_B_DIR, PWM_RANGE),
  webService(HTTP_PORT, ROOT_CTX) {
}

Esp8266::~Esp8266() {
  stop();
}

bool Esp8266::start() {

  if (!isRunning()) {
    Log.verbose(F("Setup ESP8266 ..." CR));

    wiFiService.addAP(WIFI_SSID_1, WIFI_PASSWD_1);
    wiFiService.addAP(WIFI_SSID_2, WIFI_PASSWD_2);
    wiFiService.setupWiFi();
    wiFiService.start();

    wiFiAPService.setup(WIFI_AP_SSID, WIFI_AP_PASSWD, WIFI_AP_CHANNEL, WIFI_AP_HIDDEN, WIFI_AP_MAX_CONNECTIONS);  
    wiFiAPService.enableMDNS(HOST_NAME, HTTP_PORT);
    wiFiAPService.start();

    espService.start();
    fsService.start();
    webService.start();

    // add http resources
    webService.on("/esp", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, espService.getDetails());
    });
    
    webService.on("/wifi", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, wiFiService.getDetails());
    });
    webService.on("/ap", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, wiFiAPService.getDetails());
    });
    
    webService.on("/fs/details", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, fsService.getStorageDetails());
    });
    webService.on("/fs/listing", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, fsService.getFileListing());
    });
    webService.on("/shield", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, shield.getDetails());
    });

    // add ws resource  
    webService.addWebSocket("/racer", new TrackedRacerHandler(&shield));

    running = true;

    Log.verbose(F("=========================" CR));
    Log.verbose(F("Setup finished. Have fun." CR));
    Log.verbose(F("=========================" CR));
  }

  return isRunning();
}

bool Esp8266::stop() {

  if (isRunning()) {
    webService.stop();
    fsService.stop();
    wiFiService.stop();

    running = false;
  }

  return isRunning();
}

void Esp8266::run() {

  if ((previousTime + updateInterval) < millis()) {
    previousTime = millis();

    // loop or do something else here
  }
}

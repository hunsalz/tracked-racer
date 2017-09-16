#include "Esp8266.h"

Esp8266::Esp8266() : webService(HTTP_PORT, ROOT_CTX) {
}

Esp8266::~Esp8266() {
  stop();
}

bool Esp8266::isRunning() {
  return running;
}

bool Esp8266::start() {

  if (!isRunning()) {
    Log.verbose(F("Setup ESP8266 ..." CR));

    wiFiAPService.start();
    // try to enable multicast DNS if HOST_NAME is given
    wiFiAPService.setupMDNS(HOST_NAME, 80);

    wiFiStaService.getWiFiMulti()->addAP(WIFI_SSID_1, WIFI_PASSWD_1);
    wiFiStaService.getWiFiMulti()->addAP(WIFI_SSID_2, WIFI_PASSWD_2);
    wiFiStaService.start();
    
    fsService.start();
    webService.start();

    // add http resources
    webService.on("/esp", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, getESPDetails());
    });
    webService.on("/wifi", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, wiFiStaService.getWiFiDetails());
    });
    webService.on("/fs/details", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, fsService.getStorageDetails());
    });
    webService.on("/fs/listing", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, fsService.getFileListing());
    });

    // add ws resource
    webService.addWebSocket("/racer", new TrackedRacerHandler());

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
    wiFiStaService.stop();
    wiFiAPService.stop();

    running = false;
  }

  return isRunning();
}

void Esp8266::run() {

  if ((previousTime + updateInterval) < millis()) {
    previousTime = millis();

    if (wiFiAPService.isRunning()) {
      Log.verbose(F("%d station(s) connected to soft-AP." CR), wiFiAPService.getWiFi()->softAPgetStationNum());
    }

    if (webService.isRunning()) {
      Log.verbose(F("WebServer is running." CR));
    }
  }
}

JsonObject& Esp8266::getESPDetails() {

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();
  // ESP8266 data from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h
  json[F("vcc")] = ESP.getVcc();
  json[F("heap")] = ESP.getFreeHeap();
  json[F("chipId")] = ESP.getChipId();
  json[F("sdkVersion")] = ESP.getSdkVersion();
  json[F("coreVersion")] =  ESP.getCoreVersion();
  json[F("bootVersion")] = ESP.getBootVersion();
  json[F("bootMode")] = ESP.getBootMode();
  json[F("cpuFreqMHz")] = ESP.getCpuFreqMHz();
  json[F("flashChipId")] = ESP.getFlashChipId();
  json[F("flashChipRealSize")] = ESP.getFlashChipRealSize();
  json[F("flashChipSize")] = ESP.getFlashChipSize();
  json[F("flashChipSpeed")] = ESP.getFlashChipSpeed();
  json[F("flashChipMode")] = ESP.getFlashChipMode();
  json[F("flashChipSizeByChipId")] = ESP.getFlashChipSizeByChipId();
  json[F("sketchSize")] = ESP.getSketchSize();
  json[F("sketchMD5")] = ESP.getSketchMD5();
  json[F("freeSketchSpace")] = ESP.getFreeSketchSpace();
  json[F("resetReason")] = ESP.getResetReason();
  json[F("resetInfo")] = ESP.getResetInfo();
  json[F("cycleCount")] = ESP.getCycleCount();
  json[F("uptime")] = millis();

  return json;
}

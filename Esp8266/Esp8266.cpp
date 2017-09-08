#include "Esp8266.h"

Esp8266::Esp8266() {}

Esp8266::~Esp8266() {
  stop();
}

bool Esp8266::isRunning() {
  return _running;
}

bool Esp8266::start() {

  if (!isRunning()) {
    Log.verbose(F("Setup ESP8266 ..." CR));

    wiFiAPService.start();
    wiFiStaService.start();
    fsService.start();
    webService.start();

    // add http resources
    servicesInfoHandler = &webService.getWebServer()->on("/services", HTTP_GET, getServicesInfoFunction());
    espInfoHandler = &webService.getWebServer()->on("/esp", HTTP_GET, getESPInfoFunction());
    infoHandler = &webService.getWebServer()->on("/fs/info", HTTP_GET, fsService.getInfoFunction());
    listHandler = &webService.getWebServer()->on("/fs/list", HTTP_GET, fsService.getListFunction());

    // add ws resources
    AsyncWebSocket* webSocket = new AsyncWebSocket("/racer");
    TrackedRacerHandler racerHandler = new TrackedRacerHandler(true);
    webSocket->onEvent(std::bind(&TrackedRacerHandler::onEvent, racerHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));
    &webService.getWebServer()->addHandler(webSocket);

    _running = true;

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

    _running = false;
  }

  return isRunning();
}

void Esp8266::run() {

  if ((previousTime + updateInterval) < millis()) {
    previousTime = millis();

    if (wiFiAPService.isRunning()) {
      Log.verbose(F("%d station(s) connected to soft-AP." CR), wiFiAPService.getStationNumber());
    }

    if (webService.isRunning()) {
      Log.verbose(F("WebServer is running." CR));
    }
  }
}

ArRequestHandlerFunction Esp8266::getServicesInfoFunction() {

  return [](AsyncWebServerRequest *request) {
    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject& json = response->getRoot();
    json[F("services")] = "http://" + request->host() + "/services";
    json[F("esp")] = "http://" + request->host() + "/esp";


    response->setLength();
    request->send(response);
  };
}

ArRequestHandlerFunction Esp8266::getESPInfoFunction() {

  return [](AsyncWebServerRequest *request) {
    // ESP8266 data from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.h
    AsyncJsonResponse *response = new AsyncJsonResponse();
    JsonObject& json = response->getRoot();
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

    response->setLength();
    request->send(response);
  };
}

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include "Configuration.h"
#include "Service.h"
#include "FSService.h"
#include "TrackedRacerHandler.h"
#include "WebService.h"

#include "WiFiAPService.h"
#include "WiFiStaService.h"

class Esp8266 : public Service {

	public:

    Esp8266();
    ~Esp8266();

    bool isRunning();

    bool start();

    bool stop();

		void run();
   
    JsonObject& getESPDetails();

	private:

		WiFiAPService wiFiAPService;
    WiFiStaService wiFiStaService;
		FSService fsService;
    WebService webService;

		unsigned long previousTime = millis();
		unsigned int updateInterval = 5000;

    AsyncCallbackWebHandler* servicesInfoHandler;
    AsyncCallbackWebHandler* espInfoHandler;
    AsyncCallbackWebHandler* infoHandler;
    AsyncCallbackWebHandler* listHandler;
};

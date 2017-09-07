#pragma once

#include <Arduino.h>
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include "Service.h"
#include "Configuration.h"
#include "WiFiAPService.h"
#include "WiFiStaService.h"
#include "FSService.h"
#include "WebService.h"

class Esp8266 : public Service {

	public:

    Esp8266();
    ~Esp8266();

    bool isRunning();

    bool start();

    bool stop();

		void run();

    ArRequestHandlerFunction getESPInfoFunction();

	private:

		WiFiAPService wiFiAPService;
    WiFiStaService wiFiStaService;
		FSService fsService;
    WebService webService;

    bool _running = false;
		unsigned long previousTime = millis();
		unsigned int updateInterval = 5000;

    AsyncCallbackWebHandler* espInfoHandler;
};

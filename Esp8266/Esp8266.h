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

#include <FSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/FSService.h
#include <MotorShieldDriver.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/MotorShieldDriver
#include <Service.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/Service.h
#include <WebService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebService

#include "Configuration.h"
#include "TrackedRacerHandler.h"
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

    MotorShieldDriver shield;
		WiFiAPService wiFiAPService;
    WiFiStaService wiFiStaService;
		FSService fsService;
    WebService webService;

		unsigned long previousTime = millis();
		unsigned int updateInterval = 5000;
};

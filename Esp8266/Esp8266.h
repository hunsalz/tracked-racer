#pragma once

#include <Arduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h

#include <EspService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/EspService.h
#include <FSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/FSService.h
#include <MotorShieldDriver.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/MotorShieldDriver
#include <Service.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/Service.h
#include <WebService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebService
#include <WebSocketListener.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebSocketListener
#include <WiFiAPService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WiFiAPService
#include <WiFiService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WiFiService

#include "Configuration.h"

class Esp8266 : public Service {

	public:

    Esp8266();
    ~Esp8266();

    bool start();

    bool stop();

		void run();

	private:

    MotorShieldDriver shield;
    EspService espService;
    FSService fsService;
    WiFiAPService wiFiAPService;
    WiFiService wiFiService;
    WebService webService;

		unsigned long previousTime = millis();
		unsigned int updateInterval = 5000;

    void processRequest(JsonObject &json);
    void sendResponse(AsyncWebSocketClient *client);
};

#pragma once

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266WiFiMulti.h> // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h

#include "Service.h"

class WiFiStaService : public Service {

	public:

    WiFiStaService();

    ~WiFiStaService();

    bool isRunning();

    bool start();

    bool stop();

    IPAddress getLocalIp();

  private:

		ESP8266WiFiMulti wifiMulti;

    WiFiEventHandler connectedEventHandler, disconnectedEventHandler, authModeChangedEventHandler, gotIpEventHandler;
};

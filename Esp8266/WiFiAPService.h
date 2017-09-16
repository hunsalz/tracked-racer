#pragma once

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <ESP8266mDNS.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/ESP8266mDNS.h

#ifdef ESP8266
extern "C" {
  #include "user_interface.h" // Expressif ESP8266 Non-OS API
}
#endif

#include "Configuration.h"
#include "Service.h"

class WiFiAPService : public Service {

	public:

    WiFiAPService();

    ~WiFiAPService();

    bool isRunning();

    bool start();

    bool stop();

    bool setupMDNS(const char* hostName, unsigned int port);

    ESP8266WiFiClass *getWiFi();

    /**
     * returns current configuration of the ESP8266 WiFi soft-AP
     */
    // TODO needed?
    softap_config* getConfig();

  private:

    WiFiEventHandler connectedEventHandler, disconnectedEventHandler;
};

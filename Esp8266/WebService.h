#pragma once

#include <ESPAsyncTCP.h> // https://github.com/me-no-dev/ESPAsyncTCP/blob/master/src/ESPAsyncTCP.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

class WebService : public Service {

  public:

    WebService();

    ~WebService();

    bool isRunning();

    bool start();

    bool stop();

    AsyncWebServer* getWebServer();

  private:

		AsyncWebServer webServer;

    bool _running = false;
};

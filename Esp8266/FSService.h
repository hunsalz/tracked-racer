#pragma once

#include <FS.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266/FS.h
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "Service.h"

class FSService : public Service {

	public:

    FSService();

    ~FSService();

    bool isRunning();

    bool start();

    bool stop();

    ArRequestHandlerFunction getInfoFunction();

    ArRequestHandlerFunction getListFunction();
    
    static String formatBytes(size_t bytes);

  private:

    bool _running = false;
};

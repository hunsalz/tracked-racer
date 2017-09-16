#pragma once

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h

#include "Configuration.h"
#include "MotorShieldDriver.h"
#include "WSHandler.h"

class TrackedRacerHandler : public WSHandler {

  public:

    TrackedRacerHandler();
    ~TrackedRacerHandler();

    void processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json);

    void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast);

  private:

    MotorShieldDriver *shield;
};

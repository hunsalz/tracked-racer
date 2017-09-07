#pragma once

#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h

#include "MotorShieldDriver.h"
#include "WSHandler.h"

class TrackedRacerHandler : public WSHandler {

  public:

    TrackedRacerHandler(bool enabled);

    void processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json);

    void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast);

  private:

    MotorShieldDriver *motorA;
    MotorShieldDriver *motorB;
};

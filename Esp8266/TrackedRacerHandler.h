#pragma once

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h

#include <MotorShieldDriver.h> // https://github.com/hunsalz/esp8266utils

#include "WebSocketListener.h"

class TrackedRacerHandler : public WebSocketListener {

  public:

    TrackedRacerHandler(MotorShieldDriver *shield);
    ~TrackedRacerHandler();

    void process(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json);

  private:

    MotorShieldDriver *shield;
};

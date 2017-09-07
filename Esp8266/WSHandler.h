#pragma once

#include <AsyncWebSocket.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h
#include <AsyncJson.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncJson.h
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include "Configuration.h"

class WSHandler {

  public:

    void onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

    virtual void processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json);

    virtual void notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast);

    void send(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject *json, bool broadcast);
};

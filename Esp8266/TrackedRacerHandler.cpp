#include "TrackedRacerHandler.h"

TrackedRacerHandler::TrackedRacerHandler(MotorShieldDriver *shield) {

  this->shield = shield;
  Log.verbose(F("Ready to control motor shield ..." CR));
};

void TrackedRacerHandler::processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) {

  const char* speedA = json["motorA"];
  const char* speedB = json["motorB"];
  const char* mode = json["mode"];
/**
  Log.verbose(F("motor A : %s -%d" CR), speedA, atoi(speedA));
  Log.verbose(F("motor B : %s - %d" CR), speedB, atoi(speedB));
  Log.verbose(F("mode : %s" CR), mode);
*/
  if (strcmp("absolute", mode) == 0) {
    shield->setSpeedA(atoi(speedA));
    shield->setSpeedB(atoi(speedB));
  } else {
    shield->applySpeedA(atoi(speedA));
    shield->applySpeedB(atoi(speedB));
  }

  notify(ws, client, "message", true);
}

void TrackedRacerHandler::notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast = false) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["type"] = type;
  json["clientId"] = client->id();
  json["motorA"] = shield->getSpeedA();
  json["motorB"] = shield->getSpeedB();

  send(ws, client, &json, broadcast);
}

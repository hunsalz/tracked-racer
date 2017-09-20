#include "TrackedRacerHandler.h"

TrackedRacerHandler::TrackedRacerHandler(MotorShieldDriver *shield) {

  this->shield = shield;
  Log.verbose(F("Ready to control motor shield ..." CR));
};

void TrackedRacerHandler::process(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) {

  const char* speedA = json["motorA"];
  const char* speedB = json["motorB"];
  const char* mode = json["mode"];

  Log.verbose(F("motor A : %d" CR), atoi(speedA));
  Log.verbose(F("motor B : %d" CR), atoi(speedB));
  Log.verbose(F("mode : %s" CR), mode);

  if (strcmp("absolute", mode) == 0) {
    
    Log.verbose(F("Set speed absolute ..." CR));
    
    shield->setSpeedA(atoi(speedA));
    shield->setSpeedB(atoi(speedB));
  } else {
    shield->applySpeedA(atoi(speedA));
    shield->applySpeedB(atoi(speedB));
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject& response = jsonBuffer.createObject();
  response["clientId"] = client->id();
  response["motorA"] = shield->getSpeedA();
  response["motorB"] = shield->getSpeedB();

  int length = response.measureLength() + 1;
  char payload[length];
  response.printTo(payload, length);
  client->text(payload);
}

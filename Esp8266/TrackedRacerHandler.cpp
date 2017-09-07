#include "TrackedRacerHandler.h"

TrackedRacerHandler::TrackedRacerHandler(bool enabled) {

  motorA = new MotorShieldDriver(enabled, MOTOR_A_SPEED, MOTOR_A_DIR);
  motorA->setSpeed(0);
  motorB = new MotorShieldDriver(enabled, MOTOR_B_SPEED, MOTOR_B_DIR);
  motorB->setSpeed(0);

  Log.verbose(F("Setup for remote control done." CR));
};

void TrackedRacerHandler::processMessage(AsyncWebSocket *ws, AsyncWebSocketClient *client, JsonObject &json) {

  const char* speedA = json["leftMotor"];
  const char* speedB = json["rightMotor"];
  const char* mode = json["mode"];
/**
  Log.verbose(F("motor A : %s -%d" CR), speedA, atoi(speedA));
  Log.verbose(F("motor B : %s - %d" CR), speedB, atoi(speedB));
  Log.verbose(F("mode : %s" CR), mode);
*/
  if (strcmp("absolute", mode) == 0) {
    motorA->setSpeed(atoi(speedA));
    motorB->setSpeed(atoi(speedB));
  } else {
    motorA->apply(atoi(speedA));
    motorB->apply(atoi(speedB));
  }

  notify(ws, client, "message", true);
}

void TrackedRacerHandler::notify(AsyncWebSocket *ws, AsyncWebSocketClient *client, String type, bool broadcast = false) {

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["type"] = type;
  json["clientId"] = client->id();
  json["speedA"] = motorA->getSpeed();
  json["speedB"] = motorB->getSpeed();

  send(ws, client, &json, broadcast);
}

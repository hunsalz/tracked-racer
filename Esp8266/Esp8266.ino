#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include "Esp8266.h"

Esp8266 *esp8266;

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(false);
  while(!Serial && !Serial.available()) {}

  Log.begin(LOG_LEVEL, &Serial);
  Log.verbose(F("" CR));
  
  esp8266 = new Esp8266();
  esp8266->start();
}

void loop() {
	esp8266->run();
}

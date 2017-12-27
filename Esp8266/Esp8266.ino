#include "Esp8266.h"

Esp8266* esp8266;

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(false);
  while (!Serial && !Serial.available()) {}

  LOG.verbose(F("\nSerial baud rate is [%d]"), Serial.baudRate());

  esp8266 = new Esp8266();
  esp8266->begin();
}

void loop() {
  esp8266->run();
  // time for RTOS functions
  delay(20);
}

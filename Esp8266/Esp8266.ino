#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include "Esp8266.h"

//#define DISABLE_LOGGING // uncomment to compile without any log events

Esp8266* esp8266;

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(false);
  while(!Serial && !Serial.available()) {}

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  Log.setPrefix([](Print* prefix) {
    prefix->print(F("[ESP8266] "));
  });
  
  Log.verbose(F("\nSerial baud rate is [%d]" CR), Serial.baudRate());
  
  esp8266 = new Esp8266();
  esp8266->begin();
}

void loop() {
  esp8266->run();
  // time for RTOS functions
  delay(20);
}

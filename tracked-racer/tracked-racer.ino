#include <ESP8266WebServer.h>   // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/src/ESP8266WebServer.h
#include <ESP8266mDNS.h>        // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/src/ESP8266mDNS.h
#include <StreamString.h>       // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/StreamString.h

#include <Esp8266Utils.h>       // https://github.com/hunsalz/esp8266utils

#include "config.h"

using namespace esp8266utils;

ESP8266WebServer server(80);
WebSocketsServerListener webSocketsServerListener;
WebSocketsServer webSocketsServer = WebSocketsServer(81);

MotorDriver motorA;
MotorDriver motorB;

unsigned long nextLoopInterval = 0;

void setup() {
  
  // init Serial with desired baud rate
  Logging::init(115200);
  VERBOSE_FP(F("Serial baud rate is [%lu]"), Serial.baudRate());

  // setup hardware components
  motorA.begin(MOTOR_A_PWM, MOTOR_A_DIR, MOTOR_PWM_RANGE);
  motorB.begin(MOTOR_B_PWM, MOTOR_B_DIR, MOTOR_PWM_RANGE);
  // setup PWM range once
  MotorDriver::applyPWMRange(MOTOR_PWM_RANGE);

  // WiFi AP setup
  setupWiFiAp(WIFI_AP_SSID, WIFI_AP_PSK);

  // MDNS setup
  const char* hostname = "esp8266";
  if (MDNS.begin(hostname)) {
    // add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
    VERBOSE_FP(F("MDNS enabled to http://%s.local"), hostname);
  } else {
     ERROR_FP(F("MDNS failed for http://%s.local"), hostname);
  }

  // file system setup to enable static web server content
  FileSystem fs; 
  fs.begin();

  // general web server setup
  
  // rewrite root context
  //server.rewrite("/", "/index.html");
  // handle static web resources
  //server.serveStatic("/", SPIFFS, "/www/", "max-age:15");

  // add dynamic http resources
  server.on("/fs", HTTP_GET, [&fs]() {
  
    StreamString* payload = new StreamString();
    size_t size = fs.serializeInfo(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/files", HTTP_GET, [&fs]() {
  
    StreamString* payload = new StreamString();
    size_t size = fs.serializeListing(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/ap", HTTP_GET, [&fs]() {
  
    StreamString* payload = new StreamString();
    size_t size = serializeWiFiAp(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/esp", HTTP_GET, [&fs]() {
  
    StreamString* payload = new StreamString();
    size_t size = serializeESP(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/motor_a", HTTP_GET, [&fs]() {
  
    StreamString* payload = new StreamString();
    size_t size = motorA.serialize(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/motor_b", HTTP_GET, [&fs]() {
  
    StreamString* payload = new StreamString();
    size_t size = motorB.serialize(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });

  // add static http resources
  server.on("/", []() {
    server.send_P(200, TEXT_HTML, WS_TEST_HTML);
  });

  // define specific ws listener handlers
  webSocketsServerListener.onTextMessage([](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    VERBOSE_FP(F("Payload is %s"), payload);

    webSocketsServer.sendTXT(num, "{'message': 'ok'}");
  });

  // register ws listener
  webSocketsServer.onEvent([](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    webSocketsServerListener.onEvent(num, type, payload, length);
  });

  // start services
  webSocketsServer.begin();
  server.begin();

  VERBOSE_FP(F("========================="));
  VERBOSE_FP(F("Setup finished. Have fun."));
  VERBOSE_FP(F("========================="));
}

void loop() {

  webSocketsServer.loop();
  server.handleClient();

  if (millis() > nextLoopInterval) {  
    nextLoopInterval = millis() + 15000;
    
    MDNS.update();
    VERBOSE_P(F("KEEP ALIVE"));
  }

  // reserve time for core processes
  delay(100);
}

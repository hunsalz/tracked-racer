#include <ESP8266mDNS.h>        // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/src/ESP8266mDNS.h
#include <ESP8266WebServer.h>   // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/src/ESP8266WebServer.h
#include <StreamString.h>       // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/StreamString.h

#include <Esp8266Utils.h>       // https://github.com/hunsalz/esp8266utils

#include "config.h"
#include "html.h"

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
  // add service to MDNS
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);

  // file system setup to enable static web server content
  FileSystem fs; 
  fs.begin();

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
  server.on("/ap", HTTP_GET, []() {
  
    StreamString* payload = new StreamString();
    size_t size = serializeWiFiAp(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/esp", HTTP_GET, []() {
  
    StreamString* payload = new StreamString();
    size_t size = serializeESP(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/motor_a", HTTP_GET, []() {
  
    StreamString* payload = new StreamString();
    size_t size = motorA.serialize(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });
  server.on("/motor_b", HTTP_GET, []() {
  
    StreamString* payload = new StreamString();
    size_t size = motorB.serialize(*payload);
    server.send(200, APPLICATION_JSON, *payload); 
  });

  // add static ws test resource
  server.on("/", []() {
    server.send_P(200, TEXT_HTML, WS_TEST_HTML);
  });

  // define specific ws listener handler
  webSocketsServerListener.onTextMessage([](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {

    VERBOSE_F("PAYLOAD %s", payload);
    // try to parse payload as json
    DynamicJsonDocument docRequest;
    DeserializationError err = deserializeJson(docRequest, payload);
    if (err) {
      VERBOSE_F("Reading request failed: %s", err.c_str());
      webSocketsServer.sendTXT(num, "Received an unexpected message.");
    } else {
      // map json request
      JsonObject request = docRequest.as<JsonObject>();
      int speedA = request["motorA"];
      int speedB = request["motorB"];
      const char* mode = request["mode"];
      // decide which process mode to use
      if (strcmp("absolute", mode) == 0) {
        motorA.setSpeed(speedA);
        motorB.setSpeed(speedB);
      } else {
        motorA.applySpeed(speedA);
        motorB.applySpeed(speedB);
      }
      // create json response
      DynamicJsonDocument docResponse;
      JsonObject response = docResponse.to<JsonObject>();
      response["motorA"] = motorA.getSpeed();
      response["motorB"] = motorB.getSpeed();
      // send response message
      StreamString* msg = new StreamString();
      serializeJson(response, *msg);
      webSocketsServer.sendTXT(num, *msg);
    }
  });

  // register ws listener
  webSocketsServer.onEvent([](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    webSocketsServerListener.onEvent(num, type, payload, length);
  });

  // start web services
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

    int i = webSocketsServer.connectedClients(true);
    VERBOSE_FP(F("Send WebSocket pong and received %d ping message(s)."), i);
  }

  // reserve time for core processes
  delay(100);
}

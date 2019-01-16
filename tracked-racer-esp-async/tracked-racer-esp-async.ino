#define USE_ESP_ASYNC

#include <ESP8266mDNS.h>        // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/src/ESP8266mDNS.h
#include <StreamString.h>       // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/StreamString.h

#include <Esp8266Utils.h>       // https://github.com/hunsalz/esp8266utils

#include "config.h"
#include "html.h"

using namespace esp8266utils;

MotorDriver motorA;
MotorDriver motorB;

ESPAsyncWebService webService(80);
ESPAsyncWebSocketListener wsl;

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
  MDNS.addService("ws", "tcp", 80);

  // file system setup to enable static web server content
  FileSystem fs; 
  fs.begin();

  // general web server setup
  webService.init();
  // rewrite root context
  webService.getWebServer().rewrite("/", "/index.html");
  // handle static web resources
  webService.getWebServer().serveStatic("/", SPIFFS, "/www/", "max-age:15");

  // add static ws test resource
  webService.on("/test", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, TEXT_HTML, WS_TEST_HTML);
  });

  // add dynamic http resources
  webService.on("/fs", HTTP_GET, [&fs](AsyncWebServerRequest* request) {

    AsyncResponseStream* response = request->beginResponseStream(APPLICATION_JSON);  
    StreamString* payload = new StreamString();
    size_t size = fs.serializeInfo(*payload);
    response->print(*payload); 
    request->send(response);
    delete payload;
  });
  webService.on("/files", HTTP_GET, [&fs](AsyncWebServerRequest* request) {

    AsyncResponseStream* response = request->beginResponseStream(APPLICATION_JSON);  
    StreamString* payload = new StreamString();
    size_t size = fs.serializeListing(*payload);
    response->print(*payload); 
    request->send(response);
    delete payload;
  });
  webService.on("/ap", HTTP_GET, [](AsyncWebServerRequest* request) {

    AsyncResponseStream* response = request->beginResponseStream(APPLICATION_JSON);  
    StreamString* payload = new StreamString();
    size_t size = serializeWiFiAp(*payload);
    response->print(*payload); 
    request->send(response);
    delete payload;
  });
  webService.on("/esp", HTTP_GET, [](AsyncWebServerRequest* request) {
    
    AsyncResponseStream* response = request->beginResponseStream(APPLICATION_JSON);  
    StreamString* payload = new StreamString();
    size_t size = serializeESP(*payload);
    response->print(*payload); 
    request->send(response);
    VERBOSE(*payload);
    delete payload;
  });
  webService.on("/motor_a", HTTP_GET, [](AsyncWebServerRequest* request) {
    
    AsyncResponseStream* response = request->beginResponseStream(APPLICATION_JSON);  
    StreamString* payload = new StreamString();
    size_t size = motorA.serialize(*payload);
    response->print(*payload); 
    request->send(response);
    VERBOSE(*payload);
    delete payload;
  });
  webService.on("/motor_b", HTTP_GET, [](AsyncWebServerRequest* request) {
    
    AsyncResponseStream* response = request->beginResponseStream(APPLICATION_JSON);  
    StreamString* payload = new StreamString();
    size_t size = motorB.serialize(*payload);
    response->print(*payload);
    VERBOSE(*payload);
    request->send(response);
    VERBOSE(*payload);
    delete payload;
  });
  
  // add web socket support
  wsl.onTextMessage([](AsyncWebSocket* ws, AsyncWebSocketClient* client, AwsEventType type, AwsFrameInfo* info, uint8_t* data, size_t len) {

    // extract request payload
    char payload[len + 1];
    snprintf(payload, len + 1, (char*)data);
    VERBOSE_FP(F("Payload of request is: %s"), payload);

    // try to parse payload as json
    DynamicJsonDocument docRequest;
    DeserializationError err = deserializeJson(docRequest, payload);
    if (err) {
      VERBOSE_F("Reading request failed: %s", err.c_str());
      client->text(F("Received an unexpected message."));
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
      response["clientId"] = client->id();
      response["motorA"] = motorA.getSpeed();
      response["motorB"] = motorB.getSpeed();
      // send response message
      StreamString* msg = new StreamString();
      serializeJson(response, *msg);
      client->text(*msg);
    }
  });
  // add web socket
  AsyncWebSocket* webSocket = new AsyncWebSocket("/ws");
  webSocket->onEvent([](AsyncWebSocket* ws, AsyncWebSocketClient* client, AwsEventType type, void *arg, uint8_t *data, size_t len) { 
    wsl.onEvent(ws, client, type, arg, data, len);
  });
  webService.getWebServer().addHandler(webSocket);

  webService.begin();

  VERBOSE_FP(F("========================="));
  VERBOSE_FP(F("Setup finished. Have fun."));
  VERBOSE_FP(F("========================="));
}

void loop() {

  if (millis() > nextLoopInterval) {  
    nextLoopInterval = millis() + LOOP_INTERVAL;
    
    MDNS.update();
  }

  // reserve time for core processes
  delay(500);
}

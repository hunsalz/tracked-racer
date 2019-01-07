#include <ESP8266mDNS.h>        // https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266mDNS/src/ESP8266mDNS.h
#include <ESP8266WiFiMulti.h>   // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h
#include <StreamString.h>       // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/StreamString.h

#include <Esp8266Utils.h>       // https://github.com/hunsalz/esp8266utils

#include "config.h"

esp8266utils::MotorDriver _motorA;
esp8266utils::MotorDriver _motorB;

esp8266utils::WebSocketListener _wsl;

unsigned long nextLoopInterval = 0;

void setup() {
  
  // init Serial with desired baud rate
  esp8266utils::Logging::init(115200);
  VERBOSE_FP(F("Serial baud rate is [%lu]"), Serial.baudRate());

  // setup hardware components
  _motorA.begin(MOTOR_A_PWM, MOTOR_A_DIR);
  _motorB.begin(MOTOR_B_PWM, MOTOR_B_DIR);
  //esp8266utils::setPWMRange(PWM_RANGE);

  // WiFi AP setup
  esp8266utils::setupWiFiAp(WIFI_AP_SSID, WIFI_AP_PSK);

//  // WiFi station setup
//  ESP8266WiFiMulti wifiMulti;
//  wifiMulti.addAP(WIFI_SSID_1, WIFI_PSK_1);
//  wifiMulti.addAP(WIFI_SSID_2, WIFI_PSK_2);
//  esp8266utils::setupWiFiSta(wifiMulti);

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
  esp8266utils::FileSystem fs; 
  fs.begin();

  // general web server setup
  esp8266utils::WebService webService(80);
  webService.begin();
  // rewrite root context
  webService.getWebServer().rewrite("/", "/index.html");
  // handle static web resources
  webService.getWebServer().serveStatic("/", SPIFFS, "/www/", "max-age:15");

  // add dynamic http resources
  webService.on("/fs", HTTP_GET, [&fs](AsyncWebServerRequest* request) {

    AsyncResponseStream *response = request->beginResponseStream("application/json");  
    StreamString* payload = new StreamString();
    size_t size = fs.serializeInfo(*payload);
    response->print(*payload); 
    request->send(response);
    delete payload;
  });
  webService.on("/files", HTTP_GET, [&fs](AsyncWebServerRequest* request) {

    AsyncResponseStream *response = request->beginResponseStream("application/json");  
    StreamString* payload = new StreamString();
    size_t size = fs.serializeListing(*payload);
    response->print(*payload); 
    request->send(response);
    delete payload;
  });
  webService.on("/ap", HTTP_GET, [](AsyncWebServerRequest* request) {

    AsyncResponseStream *response = request->beginResponseStream("application/json");  
    StreamString* payload = new StreamString();
    size_t size = esp8266utils::serializeWiFiAp(*payload);
    response->print(*payload); 
    request->send(response);
    delete payload;
  });
//  webService.on("/sta", HTTP_GET, [](AsyncWebServerRequest* request) {
//
//    AsyncResponseStream *response = request->beginResponseStream("application/json");  
//    StreamString* payload = new StreamString();
//    size_t size = esp8266utils::serializeWiFiSta(*payload);
//    response->print(*payload); 
//    request->send(response);
//    delete payload;
//  });
  webService.on("/esp", HTTP_GET, [](AsyncWebServerRequest* request) {
    
    AsyncResponseStream *response = request->beginResponseStream("application/json");  
    StreamString* payload = new StreamString();
    size_t size = esp8266utils::serializeESP(*payload);
    response->print(*payload); 
    request->send(response);
    VERBOSE(*payload);
    delete payload;
  });
  webService.on("/motor_a", HTTP_GET, [](AsyncWebServerRequest* request) {
    
    AsyncResponseStream *response = request->beginResponseStream("application/json");  
    StreamString* payload = new StreamString();
    size_t size = _motorA.serialize(*payload);
    response->print(*payload); 
    request->send(response);
    VERBOSE(*payload);
    delete payload;
  });
  webService.on("/motor_b", HTTP_GET, [](AsyncWebServerRequest* request) {
    
    AsyncResponseStream *response = request->beginResponseStream("application/json");  
    StreamString* payload = new StreamString();
    size_t size = _motorB.serialize(*payload);
    response->print(*payload);
    VERBOSE(*payload);
    request->send(response);
    VERBOSE(*payload);
    delete payload;
  });
  
  // add web socket support
  _wsl.onTextMessage([](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

    // extract request payload
    char payload[len + 1];
    snprintf(payload, len + 1, (char*)data);
    VERBOSE_FP(F("Payload of request is: %s"), payload);

    client->text("DONE");
    return;

    // try to parse payload as JSON
    DynamicJsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err) {
      VERBOSE_F("Reading request failed: %s", err.c_str());
      client->text(F("Received an unexpected message."));
    } else {
//      JsonObject request = doc.as<JsonObject>();
//      int speedA = request["motorA"];
//      int speedB = request["motorB"];
//      char* mode = request["mode"];
//      // decide which process mode to use
//      if (strcmp("absolute", mode) == 0) {
//        _motorA.setSpeed(speedA);
//        _motorB.setSpeed(speedB);
//      } else {
//        _motorA.applySpeed(speedA);
//        _motorB.applySpeed(speedB);
//      }

      
      // create JSON response
//      DynamicJsonDocument doc2;
//      JsonObject response = doc2.to<JsonObject>();
//      response["clientId"] = client->id();
//      response["motorA"] = _motorA.getSpeed();
//      response["motorB"] = _motorB.getSpeed();

//      StreamString* payload = new StreamString();
//      serializeJson(response, payload);
//      size_t size = measureJson(response);
      //client->text(*payload);

//      // send reply message
//      uint16_t length = message.measureLength() + 1;
//      char payload[length];
//      message.printTo(payload, length);
//      client->text(payload);
    }
  });
  // add web socket
  AsyncWebSocket *webSocket = new AsyncWebSocket("/racer");
  webSocket->onEvent([](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { 
    _wsl.onEvent(ws, client, type, arg, data, len);
  });
  webService.getWebServer().addHandler(webSocket);

  VERBOSE_FP(F("========================="));
  VERBOSE_FP(F("Setup finished. Have fun."));
  VERBOSE_FP(F("========================="));
}

void loop() {

  if (millis() > nextLoopInterval) {  
    nextLoopInterval = millis() + 5000;
    
    MDNS.update();
  }

  // reserve time for core processes
  delay(500);
}

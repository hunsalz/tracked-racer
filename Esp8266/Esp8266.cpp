#include "Esp8266.h"

void Esp8266::begin() {

  LOG.verbose(F("Setup ESP8266 ..."));
  // setup hardware components
  _motorA.begin(MOTOR_A_PWM, MOTOR_A_DIR),
  _motorB.begin(MOTOR_B_PWM, MOTOR_B_DIR),
  MotorDriver::setPWMRange(PWM_RANGE);
  // setup WiFi client
  WIFI_CLIENT.getWiFiMulti().addAP(WIFI_SSID_1, WIFI_PASSWD_1);
  WIFI_CLIENT.getWiFiMulti().addAP(WIFI_SSID_2, WIFI_PASSWD_2);
  WIFI_CLIENT.begin();
  // setup WiFi access point
  WIFI_STATION.begin(WIFI_AP_SSID, WIFI_AP_PASSWD);
  // setup MDNS
  MDNS_SERVICE.begin("esp8266");
  MDNS_SERVICE.getMDNSResponder().addService("http", "tcp", PORT);
  //MDNS_SERVICE.getMDNSResponder().addService("https", "tcp", 443);
  FILESYSTEM.getFileSystem();
  // setup web server
  SERVER.begin(PORT);
  // rewrite root context˘˘
  SERVER.getWebServer().rewrite("/", "/index.build.html");
  // handle static web resources
  SERVER.getWebServer().serveStatic("/", SPIFFS, "/www/", "max-age:15"); // cache-control 15 seconds
  // add dynamic http resources
  SERVER.on("/esp", HTTP_GET, [this](AsyncWebServerRequest * request) {
    SERVER.send(request, SYSTEM.getDetails());
  });
  SERVER.on("/fs/details", HTTP_GET, [this](AsyncWebServerRequest * request) {
    SERVER.send(request, FILESYSTEM.getStorageDetails());
  });
  SERVER.on("/fs/listing", HTTP_GET, [this](AsyncWebServerRequest * request) {
    SERVER.send(request, FILESYSTEM.getFileListing());
  });
  SERVER.on("/wifi/client", HTTP_GET, [this](AsyncWebServerRequest * request) {
    SERVER.send(request, WIFI_CLIENT.getDetails());
  });
  SERVER.on("/wifi/station", HTTP_GET, [this](AsyncWebServerRequest * request) {
    SERVER.send(request, WIFI_STATION.getDetails());
  });
  SERVER.on("/mdns", HTTP_GET, [this](AsyncWebServerRequest * request) {
    SERVER.send(request, MDNS_SERVICE.getDetails());
  });
  SERVER.on("/motor/a", HTTP_GET, [this](AsyncWebServerRequest *request) {
    SERVER.send(request, _motorA.getDetails());
  });
  SERVER.on("/motor/b", HTTP_GET, [this](AsyncWebServerRequest *request) {
    SERVER.send(request, _motorB.getDetails());
  }); 
  // add web socket support
  _wsl.onTextMessage([this](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

    DynamicJsonBuffer buffer;
    JsonObject &json = buffer.parse((char*)data);
    if (json.success() && json["motorA"].success() && json["motorB"].success() && json["mode"].success()) {

      int speedA = json["motorA"];
      int speedB = json["motorB"];
      
      //int speedA = atoi(json["motorA"]);
      //int speedB = atoi(json["motorB"]);
      const char* mode = json["mode"];

      LOG.verbose(F("Set motor A = %d and motor B = %d, PWM range = %d"), speedA, speedB, _motorA.getPWMRange());

      // decide which mode to use
      if (strcmp("absolute", mode) == 0) {
        _motorA.setSpeed(speedA);
        _motorB.setSpeed(speedB);
      } else {
        _motorA.applySpeed(speedA);
        _motorB.applySpeed(speedB);
      }

      // create JSON reply message
      DynamicJsonBuffer buffer;
      JsonObject& message = buffer.createObject();
      message["clientId"] = client->id();
      message["motorA"] = _motorA.getSpeed();
      message["motorB"] = _motorB.getSpeed();

      // send reply message
      uint16_t length = message.measureLength() + 1;
      char payload[length];
      message.printTo(payload, length);
      client->text(payload);
    } else {
      client->text(F("Received an unexpected message."));
    }
  });
  // add web socket
  AsyncWebSocket* webSocket = new AsyncWebSocket("/racer");
  webSocket->onEvent([this](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    _wsl.onEvent(ws, client, type, arg, data, len);
  });
  SERVER.getWebServer().addHandler(webSocket);
  
  LOG.verbose(F("========================="));
  LOG.verbose(F("Setup finished. Have fun."));
  LOG.verbose(F("========================="));
}

void Esp8266::run() {

  if (SYSTEM.nextLoopInterval()) {

    MDNS_SERVICE.getMDNSResponder().update();
  }
}
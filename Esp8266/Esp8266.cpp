#include "Esp8266.h"

Esp8266::Esp8266() :
  webService(80, "/index.build.html") {
}

Esp8266::~Esp8266() {
  stop();
}

bool Esp8266::isRunning() {
  return running;
}

bool Esp8266::start() {
  
  if (!isRunning()) {
    Log.verbose(F("Setup ESP8266 ..." CR));

    // setup hardware components
    motorA.setup(MOTOR_A_PWM, MOTOR_A_DIR, PWM_RANGE),
    motorB.setup(MOTOR_B_PWM, MOTOR_B_DIR, PWM_RANGE),

    // setup wiFi
    wiFiService.addAP(WIFI_SSID_1, WIFI_PASSWD_1);
    wiFiService.addAP(WIFI_SSID_2, WIFI_PASSWD_2);
    wiFiService.setupWiFi();
    wiFiService.start();

    wiFiAPService.enableMDNS("esp8266", 80);

    wiFiAPService.setup(WIFI_AP_SSID, WIFI_AP_PASSWD);
    wiFiAPService.start();

    // setup further services
    espService.start();
    fsService.start();
    webService.start();

    // add http resources
    webService.on("/esp", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, espService.getDetails());
    });
    webService.on("/wifi", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, wiFiService.getDetails());
    });
    webService.on("/ap", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, wiFiAPService.getDetails());
    });
    webService.on("/fs/details", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, fsService.getStorageDetails());
    });
    webService.on("/fs/listing", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, fsService.getFileListing());
    });
    webService.on("/motor/a", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, motorA.getDetails());
    });
    webService.on("/motor/b", HTTP_GET, [this](AsyncWebServerRequest *request) {
      webService.send(request, motorB.getDetails());
    });

    // implement controls of onTextMessage(...)
    wsl.onTextMessage([this](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

      DynamicJsonBuffer buffer;
      JsonVariant variant = buffer.parse((char*)data);   
      if (variant.is<JsonObject&>()) {

        // process JSON instructions
        JsonObject &json = variant.as<JsonObject&>();
        int speedA = atoi(json["motorA"]);
        int speedB = atoi(json["motorB"]);
        const char* mode = json["mode"];
      
        Log.verbose(F("Set motor A = %d and motor B = %d, PWM range = %d" CR), speedA, speedB, motorA.getPWMRange());

        // decide which mode to use
        if (strcmp("absolute", mode) == 0) {
          motorA.setSpeed(speedA);
          motorB.setSpeed(speedB);
        } else {
          motorA.applySpeed(speedA);
          motorB.applySpeed(speedB);
        }

        // create reply message
        DynamicJsonBuffer buffer;
        JsonObject& message = buffer.createObject();
        message["clientId"] = client->id();
        message["motorA"] = motorA.getSpeed();
        message["motorB"] = motorB.getSpeed();

        // send reply message
        uint16_t length = message.measureLength() + 1;
        char payload[length];
        message.printTo(payload, length);
        client->text(payload);            
      } else {
        client->text(F("Unexpected message"));
      }
    });

    // create WebSocket
    AsyncWebSocket* webSocket = new AsyncWebSocket("/racer");
    webSocket->onEvent([this](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
      wsl.onEvent(ws, client, type, arg, data, len);
    });
    webService.getWebServer()->addHandler(webSocket);

    running = true;

    Log.verbose(F("=========================" CR));
    Log.verbose(F("Setup finished. Have fun." CR));
    Log.verbose(F("=========================" CR));
  }

  return isRunning();
}

bool Esp8266::stop() {

  if (isRunning()) {
    webService.stop();
    fsService.stop();
    wiFiService.stop();

    running = false;
  }

  return isRunning();
}

void Esp8266::run() {

  if ((previousTime + UPDATE_INTERVAL) < millis()) {
    previousTime = millis();

    // do something else here
  }
}

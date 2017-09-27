#include "Esp8266.h"

Esp8266::Esp8266() :
  motorA(true, MOTOR_A_PWM, MOTOR_A_DIR, PWM_RANGE),
  motorB(true, MOTOR_B_PWM, MOTOR_B_DIR, PWM_RANGE),
  webService(HTTP_PORT, ROOT_CTX) {
}

Esp8266::~Esp8266() {
  stop();
}

bool Esp8266::start() {
  
  if (!isRunning()) {
    Log.verbose(F("Setup ESP8266 ..." CR));

    wiFiService.addAP(WIFI_SSID_1, WIFI_PASSWD_1);
    wiFiService.addAP(WIFI_SSID_2, WIFI_PASSWD_2);
    wiFiService.setupWiFi();
    wiFiService.start();

    wiFiAPService.enableMDNS(HOST_NAME, HTTP_PORT);

    wiFiAPService.setup(WIFI_AP_SSID, WIFI_AP_PASSWD, WIFI_AP_CHANNEL, WIFI_AP_HIDDEN, WIFI_AP_MAX_CONNECTIONS);
    wiFiAPService.start();

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

    // implement control commands received by WebSocket connection
    wsl.onTextMessage([this](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

      DynamicJsonBuffer buffer;
      JsonVariant variant = buffer.parse((char*)data);   
      if (variant.is<JsonObject&>()) {

        JsonObject &json = variant.as<JsonObject&>();
        // process JSON instructions
        int speedA = atoi(json["motorA"]);
        int speedB = atoi(json["motorB"]);
        const char* mode = json["mode"];
      
        Log.verbose(F("A = %d and B = %d pwmRange = %d" CR), speedA, speedB, motorA.getPWMRange());
      
        if (strcmp("absolute", mode) == 0) {
          motorA.setSpeed(speedA);
          motorB.setSpeed(speedB);
        } else {
          motorA.applySpeed(speedA);
          motorB.applySpeed(speedB);
        }

        // send reply to client
        DynamicJsonBuffer buffer;
        JsonObject& reply = buffer.createObject();
        reply["clientId"] = client->id();
        reply["motorA"] = motorA.getSpeed();
        reply["motorB"] = motorB.getSpeed();

        // TODO utility function object json -> char[]

        int length = reply.measureLength() + 1;
        char payload[length];
        reply.printTo(payload, length);
        client->text(payload);
              
      } else {
        // TODO send payload too?
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

  if ((previousTime + updateInterval) < millis()) {
    previousTime = millis();

    // continous loop or do something else here
  }
}

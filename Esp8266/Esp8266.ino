#include <Esp8266Utils.h>  // https://github.com/hunsalz/esp8266utils
#include <Log4Esp.h>       // https://github.com/hunsalz/log4Esp

#include "config.h"

// web server settings
const static int PORT = 80;

esp8266util::MotorDriver _motorA;
esp8266util::MotorDriver _motorB;

esp8266util::WebSocketListener _wsl;

void setup() {
  // logger setup
  LOG.addLevelToAll(Appender::VERBOSE);
  LOG.addFormatterToAll(
      [](Print &output, Appender::Level level, const char *msg, va_list *args) {
        // output log level
        output.print(Appender::toString(level, true));
        output.print(Appender::DEFAULT_SEPARATOR);
        // output uptime of this program in milliseconds
        output.print(millis());
        output.print(Appender::DEFAULT_SEPARATOR);
        // output free heap space
        output.print(ESP.getFreeHeap());
        output.print(Appender::DEFAULT_SEPARATOR);
        // determine buffer length for formatted data
        size_t length = vsnprintf(NULL, 0, msg, *args) + 1;
        char buffer[length];
        // output formatted data
        vsnprintf(buffer, length, msg, *args);
        output.print(buffer);
      });

  // serial setup
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  while (!Serial && !Serial.available()) {
  };
  Serial.println();
  LOG.verbose(F("Serial baud rate is [%d]"), Serial.baudRate());

  // setup hardware components
  _motorA.begin(MOTOR_A_PWM, MOTOR_A_DIR),
      _motorB.begin(MOTOR_B_PWM, MOTOR_B_DIR),
      esp8266util::MotorDriver::setPWMRange(PWM_RANGE);

  // WiFi setup
  WIFI_STA_CFG.addAP(WIFI_SSID_1, WIFI_PSK_1);
  WIFI_STA_CFG.addAP(WIFI_SSID_2, WIFI_PSK_2);
  WIFI_STA_CFG.begin();

  WIFI_AP_CFG.begin(WIFI_AP_SSID, WIFI_AP_PSK);

  // MDNS setup
  MDNS_CFG.begin("esp8266");
  MDNS.addService("http", "tcp", PORT);

  // increase loop interval
  SYS_CFG.setLoopInterval(20000);

  // file system setup to enable static web server content
  FILESYSTEM.begin();

  // web server setup
  SERVER.begin();
  // rewrite root context
  SERVER.getWebServer().rewrite("/", "/index.build.html");
  // handle static web resources
  SERVER.getWebServer().serveStatic("/", SPIFFS, "/www/", "max-age:15");
  // cache-control 15 seconds
  // add dynamic http resources
  SERVER.on("/fs", HTTP_GET, [](AsyncWebServerRequest *request) {
    SERVER.send(request, FILESYSTEM.getStorageDetails());
  });
  SERVER.on("/files", HTTP_GET, [](AsyncWebServerRequest *request) {
    SERVER.send(request, FILESYSTEM.getFileListing());
  });
  SERVER.on("/sta", HTTP_GET, [](AsyncWebServerRequest *request) {
    SERVER.send(request, WIFI_STA_CFG.getDetails());
  });
  SERVER.on("/ap", HTTP_GET, [](AsyncWebServerRequest *request) {
    SERVER.send(request, WIFI_AP_CFG.getDetails());
  });
  SERVER.on("/esp", HTTP_GET, [](AsyncWebServerRequest *request) {
    SERVER.send(request, SYS_CFG.getDetails());
  });
  SERVER.on("/motor/a", HTTP_GET, [](AsyncWebServerRequest *request) {
    SERVER.send(request, _motorA.getDetails());
  });
  SERVER.on("/motor/b", HTTP_GET, [](AsyncWebServerRequest *request) {
    SERVER.send(request, _motorB.getDetails());
  });
  // add web socket support
  _wsl.onTextMessage([](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, AwsFrameInfo *info, uint8_t *data, size_t len) {

    DynamicJsonBuffer buffer;
    JsonObject &json = buffer.parse((char *)data);

    if (json.success() && json["motorA"].success() && json["motorB"].success() && json["mode"].success()) {
      int speedA = json["motorA"];
      int speedB = json["motorB"];

      const char *mode = json["mode"];

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
      JsonObject &message = buffer.createObject();
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
  AsyncWebSocket *webSocket = new AsyncWebSocket("/racer");
  webSocket->onEvent([](AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { 
    _wsl.onEvent(ws, client, type, arg, data, len);
  });
  SERVER.getWebServer().addHandler(webSocket);

  LOG.verbose(F("========================="));
  LOG.verbose(F("Setup finished. Have fun."));
  LOG.verbose(F("========================="));
}

void loop() {
  if (SYS_CFG.nextLoopInterval()) {
    MDNS.update();
  }
}
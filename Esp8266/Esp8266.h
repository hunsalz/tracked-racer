#pragma once

#include <Arduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log
#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/ESPAsyncWebServer.h

#include <EspService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/EspService.h
#include <FSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/FSService.h
#include <MDNSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/MDNSService.h
#include <MotorDriver.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/MotorShieldDriver
#include <Service.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/Service.h
#include <WebService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebService
#include <WebSocketListener.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebSocketListener
#include <WiFiAPService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WiFiAPService
#include <WiFiService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WiFiService

class Esp8266 : public esp8266util::Service {

	public:

    Esp8266();
    ~Esp8266();

    bool isRunning();

    bool start();

    bool stop();

		void run();

	private:

    esp8266util::MotorDriver motorA;
    esp8266util::MotorDriver motorB;
    esp8266util::EspService espService;
    esp8266util::FSService fsService;
    esp8266util::MDNSService mdnsService;
    esp8266util::WiFiAPService wiFiAPService;
    esp8266util::WiFiService wiFiService;
    esp8266util::WebService webService;
    esp8266util::WebSocketListener wsl;

    bool running = false;

		unsigned long previousTime = millis();

    // const definitions
    
		const uint16_t UPDATE_INTERVAL = 5000;

    const uint8_t PWM_RANGE = 25;
    
    // pin definition for motor shield
    const uint8_t MOTOR_A_PWM = 5; // D1
    const uint8_t MOTOR_A_DIR = 0; // D3
    const uint8_t MOTOR_B_PWM = 4; // D2
    const uint8_t MOTOR_B_DIR = 2; // D4

    // wiFi settings
    const char* WIFI_SSID_1 = "***";
    const char* WIFI_PASSWD_1 = "***";
    const char* WIFI_SSID_2 = "***";
    const char* WIFI_PASSWD_2 = "***";

    const char* WIFI_AP_SSID = "MyESP8266";
    const char* WIFI_AP_PASSWD = "password";
};

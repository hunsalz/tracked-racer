#pragma once

#include <Arduino.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ArduinoLog.h> // https://github.com/thijse/Arduino-Log

#include <EspService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/EspService.h
#include <FSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/FSService.h
#include <Service.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/Service.h
#include <WebService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebService.h
#include <WebSocketListener.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebSocketListener.h

#include <actuator/MotorDriver.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/actuator/MotorShieldDriver.h
#include <logging/LogService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/logging/LogService.h
#include <network/MDNSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/network/MDNSService.h
#include <network/WiFiAPService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/network/WiFiAPService.h
#include <network/WiFiService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/network/WiFiService.h

using esp8266util::MotorDriver;
using esp8266util::FILESYSTEM;
using esp8266util::MDNS_SERVICE;
using esp8266util::SERVER;
using esp8266util::SYSTEM;
using esp8266util::WIFI_CLIENT;
using esp8266util::WIFI_STATION;

class Esp8266 {

	public:

    void begin();

		void run();

	private: 

    esp8266util::MotorDriver motorA;
    esp8266util::MotorDriver motorB;

    esp8266util::LogService logService;
    esp8266util::WebSocketListener wsl;
     
		unsigned long previousTime = millis();

    // const definitions

		const static uint16_t UPDATE_INTERVAL = 5000;

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

    // web server settings
    const static int PORT = 80;
};

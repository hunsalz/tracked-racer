#pragma once

#include <Arduino.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include <Logger.h> // https://github.com/hunsalz/log4arduino

#include <EspService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/EspService.h
#include <FSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/FSService.h
#include <network/MDNSService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/network/MDNSService.h
#include <network/MQTTService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/network/MQTTService.h
#include <network/WiFiAPService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/network/WiFiAPService.h
#include <network/WiFiService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/network/WiFiService.h
#include <WebService.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebService.h
#include <WebSocketListener.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/WebSocketListener.h

#include <actuator/MotorDriver.h> // https://github.com/hunsalz/esp8266utils/blob/master/src/actuator/MotorDriver.h

using esp8266util::MotorDriver;
using log4arduino::LOG;
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

    esp8266util::WebSocketListener wsl;
    
    const uint8_t PWM_RANGE = 25;

    // pin definition for motor shield
    const uint8_t MOTOR_A_PWM = 5; // D1
    const uint8_t MOTOR_A_DIR = 0; // D3
    const uint8_t MOTOR_B_PWM = 4; // D2
    const uint8_t MOTOR_B_DIR = 2; // D4

    // wiFi settings
    const char* WIFI_SSID_1 = "xxx";
    const char* WIFI_PASSWD_1 = "xxx";
    const char* WIFI_SSID_2 = "xxx";
    const char* WIFI_PASSWD_2 = "xxx";

    const char* WIFI_AP_SSID = "MyESP8266";
    const char* WIFI_AP_PASSWD = "password";

    // web server settings
    const static int PORT = 80;
};

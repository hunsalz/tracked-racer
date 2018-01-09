#pragma once

#include <Arduino.h> // https://github.com/esp8266/Arduino/tree/master/cores/esp8266
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include <Log4Esp.h> // https://github.com/hunsalz/log4Esp
#include <Esp8266Utils.h> // https://github.com/hunsalz/esp8266utils

using esp8266util::MotorDriver;
using log4Esp::LOG;
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

    esp8266util::MotorDriver _motorA;
    esp8266util::MotorDriver _motorB;

    esp8266util::WebSocketListener _wsl;
    
    const uint8_t PWM_RANGE = 25;

    // pin definition for motor shield
    const uint8_t MOTOR_A_PWM = 5; // D1
    const uint8_t MOTOR_A_DIR = 0; // D3
    const uint8_t MOTOR_B_PWM = 4; // D2
    const uint8_t MOTOR_B_DIR = 2; // D4

    // wiFi settings

    // const char* WIFI_SSID_1 = "xxx";
    // const char* WIFI_PASSWD_1 = "xxx";
    // const char* WIFI_SSID_2 = "xxx";
    // const char* WIFI_PASSWD_2 = "xxx";

    const char* WIFI_AP_SSID = "MyESP8266";
    const char* WIFI_AP_PASSWD = "password";

    // web server settings
    const static int PORT = 80;
};

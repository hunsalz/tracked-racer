#pragma once

//#define DISABLE_LOGGING // uncomment to compile without any log events
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define PWM_RANGE 25

// pin definition for motor shield
#define MOTOR_A_PWM 5 // D1
#define MOTOR_A_DIR 0  // D3
#define MOTOR_B_PWM 4 // D2
#define MOTOR_B_DIR 2 // D4

#define WIFI_SSID_1 "xxx"
#define WIFI_PASSWD_1 "xxx"
#define WIFI_SSID_2 "xxx"
#define WIFI_PASSWD_2 "xxx"

#define WIFI_AP_SSID "MyESP8266"
#define WIFI_AP_PASSWD "password"
#define WIFI_AP_CHANNEL 1
#define WIFI_AP_HIDDEN 0
#define WIFI_AP_MAX_CONNECTIONS 4

#define HOST_NAME "esp8266"

#define HTTP_PORT 80
#define ROOT_CTX "/index.build.html"

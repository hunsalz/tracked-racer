#pragma once

//#define DISABLE_LOGGING // uncomment to compile without any log events
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define PWM_RANGE 25

// pin definitions for L298N motor driver depending on individual hardware setup
#define PIN_L298N_ENA 5 // D1
#define PIN_L298N_IN1 4 // D2
#define PIN_L298N_IN2 0 // D3
#define PIN_L298N_ENB 14 // D5
#define PIN_L298N_IN3 12 // D6
#define PIN_L298N_IN4 13 // D7

// pin definition for motor shield
#define MOTOR_A_SPEED 5 // D1
#define MOTOR_A_DIR 0  // D3
#define MOTOR_B_SPEED 4 // D2
#define MOTOR_B_DIR 2 // D4



#define WIFI_AP_SSID "MyESP8266"
#define WIFI_AP_PASSWD "password"
#define WIFI_AP_CHANNEL 1
#define WIFI_AP_HIDDEN 0
#define WIFI_AP_MAX_CONNECTIONS 4

#define HOST_NAME "esp8266"

#define HTTP_PORT 80
#define ROOT_CTX "/index.build.html"

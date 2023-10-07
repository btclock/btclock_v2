#pragma once

#include <Arduino.h>
//##include <Crypto.h>

#include <WiFi.h>
#include <map>

#include <WiFiClientSecure.h>
#ifndef NO_MCP
#include <Adafruit_MCP23X17.h>
#endif
#ifdef WITH_RGB_LED
#include <Adafruit_NeoPixel.h>
#endif
#include <ESP32Time.h>
#include <Preferences.h>

typedef std::function<void()> EventCallback;
typedef std::function<void(uint number)> EventCallbackWithNumber;

extern WiFiClient wifiClientInsecure;
extern WiFiClientSecure wifiClient;

extern ESP32Time rtc;
extern Preferences preferences;

extern String softAP_SSID;
extern String softAP_password;

extern bool timerRunning;
extern uint timerSeconds;
extern uint32_t moment;

#ifndef NO_MCP
extern Adafruit_MCP23X17 mcp;
extern const int MCP_INT_PIN;
#endif
#ifdef WITH_RGB_LED
extern Adafruit_NeoPixel pixels;
#endif
extern std::map<int, std::string> screenNameMap;

const PROGMEM int SCREEN_BLOCK_HEIGHT = 0;
const PROGMEM int SCREEN_MSCW_TIME = 1;
const PROGMEM int SCREEN_BTC_TICKER = 2;
const PROGMEM int SCREEN_TIME = 3;
const PROGMEM int SCREEN_HALVING_COUNTDOWN = 4;
const PROGMEM int SCREEN_COUNTDOWN = 98;
const PROGMEM int SCREEN_CUSTOM = 99;
const PROGMEM int screens[5] = { SCREEN_BLOCK_HEIGHT, SCREEN_MSCW_TIME, SCREEN_BTC_TICKER, SCREEN_TIME, SCREEN_HALVING_COUNTDOWN };

const uint screenCount = sizeof(screens) / sizeof(int);
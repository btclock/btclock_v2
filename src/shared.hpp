#pragma once

#include "config.h"
#include <Arduino.h>
//##include <Crypto.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <map>

#ifdef IS_BW
#include <GxEPD2_BW.h>
#else 
#include <GxEPD2_3C.h>
#endif

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

//extern WiFiClient wifiClientInsecure;
extern WiFiClientSecure wifiClient;

extern ESP32Time rtc;
extern Preferences preferences;

extern String softAP_SSID;
extern String softAP_password;

extern bool timerRunning;
extern uint timerSeconds;
extern uint32_t moment;

extern GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> displays[NUM_SCREENS];

#ifndef NO_MCP
extern Adafruit_MCP23X17 mcp;
extern const char MCP_INT_PIN;
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

struct SpiRamAllocator {
  void* allocate(size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
  }

  void deallocate(void* pointer) {
    heap_caps_free(pointer);
  }

  void* reallocate(void* ptr, size_t new_size) {
    return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
  }
};

using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;

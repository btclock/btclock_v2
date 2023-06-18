#include <Arduino.h>
#include <config.h>
#include <shared.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
//#include <ESP32Time.h>
#include <WiFiManager.h>

#ifdef CONFIG_BT_ENABLED
#include "esp_bt.h"
#include "esp_bt_main.h"
#endif

#include "lib/functions.hpp"
#include "lib/webserver.hpp"

#include "screens/time.hpp"
#include "screens/blockheight.hpp"
#include "screens/ticker.hpp"
#include "screens/time.hpp"
#include "screens/sats_per_dollar.hpp"
#include "screens/halvingcountdown.hpp"

#include "tasks/ha.hpp"
#include "tasks/epd.hpp"
#include "tasks/button.hpp"
#include "tasks/led_handler.hpp"

WiFiClient wifiClientInsecure;
WiFiClientSecure wifiClient;
ESP32Time rtc(3600);

void setup()
{
  Serial.begin(115200);

#ifdef ARDUINO_ESP32S3_DEV
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

#ifdef CONFIG_BT_ENABLED
  esp_bluedroid_disable();
  esp_bluedroid_deinit();
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
#endif

  setupComponents();
  setupPreferences();
#ifndef NO_DISPLAY
  resetAllDisplays();
  initDisplays();
#endif
  setupWifi();

  synchronizeTime();
  setupWebserver();

  TimeScreen::init();
  BlockHeightScreen::init();
  TickerScreen::init();
  SatsPerDollarScreen::init();

#ifdef WITH_BUTTONS
  setupButtonTask();
#endif

#ifdef WITH_RGB_LED
  setLights(0,0,0);
  setupLedHandlerTask();
#endif

  registerNewMinuteCallback(TimeScreen::onNewMinute);
  registerNewBlockCallback(BlockHeightScreen::onNewBlock);
  registerNewBlockCallback(HalvingCountdownScreen::onNewBlock);
  registerNewPriceCallback(TickerScreen::onPriceUpdate);
  registerNewPriceCallback(SatsPerDollarScreen::onPriceUpdate);

  setupDisplays();
}

void loop()
{
  // put your main code here, to run repeatedly:
}

#include <Arduino.h>
#include <config.h>
#include <shared.hpp>
#include <WiFi.h>
#include <WiFiClientSecure.h>
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
  initDisplays();
#endif
  setupWifi();

  bool slaveMode = preferences.getBool("I2CSlaveMode", false);

  if (!slaveMode)
  {
    synchronizeTime();
    setupWebserver();

    TimeScreen::init();
    BlockHeightScreen::init();
    HalvingCountdownScreen::init();
    TickerScreen::init();

#ifdef WITH_BUTTONS
    setupButtonTask();
#endif

#ifdef WITH_RGB_LED
    setLights(0, 0, 0);
    setupLedHandlerTask();
#endif

    registerNewMinuteCallback(TimeScreen::onNewMinute);
    registerNewBlockCallback(BlockHeightScreen::onNewBlock);
    registerNewBlockCallback(HalvingCountdownScreen::onNewBlock);
    registerNewPriceCallback(TickerScreen::onPriceUpdate);

    setupDisplays();
  }
  else
  {
    setupI2C();
  }
}

void loop()
{
}

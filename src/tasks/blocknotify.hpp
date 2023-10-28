#pragma once

#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "base64.h"
#include "lib/functions.hpp"

#include "shared.hpp"
#include "config.h"

typedef struct {
  String event;
  int blockHeight;
} BitcoinEvent;

extern TaskHandle_t blockNotifyTaskHandle;

void checkBitcoinBlock(void *pvParameters);
//void bitcoinEventHandler(void *pvParameters);
void setupBlockNotify();

void registerNewBlockCallback(EventCallbackWithNumber cb);
int getBlockFromBitcoind();
int getBlockFromMempoolSpace();
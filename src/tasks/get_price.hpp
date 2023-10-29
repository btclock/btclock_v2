#pragma once

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "shared.hpp"
#include "config.h"
#include "lib/functions.hpp"

extern TaskHandle_t getPriceTaskHandle;

void taskGetPrice(void *pvParameters);
void setupGetPriceTask();
void registerNewPriceCallback(const EventCallbackWithNumber cb);
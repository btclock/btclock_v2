#pragma once

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "shared.hpp"
#include "config.h"

extern TaskHandle_t getPriceTaskHandle;

void taskGetPrice(void *pvParameters);
void setupGetPriceTask();
void registerNewPriceCallback(EventCallbackWithNumber cb);
#pragma once

#ifndef NO_MCP
#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <vector>
#include "shared.hpp"
#include "lib/functions.hpp"

extern TaskHandle_t buttonTaskHandle;


void buttonTask(void *pvParameters);
void setupButtonTask();
void registerNewButtonCallback(EventCallback cb);
void IRAM_ATTR handleButtonInterrupt();

#endif
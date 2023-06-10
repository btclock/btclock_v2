#pragma once
#ifdef WITH_RGB_LED
#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <vector>
#include "shared.hpp"

extern TaskHandle_t ledHandlerTaskHandle;

void ledHandlerTask(void *pvParameters);
void setupLedHandlerTask();
#endif
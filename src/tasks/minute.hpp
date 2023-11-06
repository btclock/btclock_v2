#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <vector>
#include "shared.hpp"
#include <esp_timer.h>
#include <time.h>

extern TaskHandle_t minuteTaskHandle;

void minuteTask(void *pvParameters);
void setupMinuteEvent();
void IRAM_ATTR minuteTimerISR(void* arg);

void registerNewMinuteCallback(const EventCallback cb);
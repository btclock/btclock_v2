#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <vector>
#include "shared.hpp"

extern TaskHandle_t minuteTaskHandle;

void minuteTask(void *pvParameters);
void setupMinuteEvent();
void registerNewMinuteCallback(const EventCallback cb);
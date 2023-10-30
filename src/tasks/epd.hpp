#pragma once

#ifdef IS_BW
#include <GxEPD2_BW.h>
#else 
#include <GxEPD2_3C.h>
#endif


#include <string>
#include "screens/blockheight.hpp"
#include "screens/ticker.hpp"
#include "screens/time.hpp"
#include "screens/sats_per_dollar.hpp"
#include "screens/countdown.hpp"
#include "screens/custom_text.hpp"
#include "screens/halvingcountdown.hpp"
#include <native_pin.hpp>
#include <mcp23x17_pin.hpp>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "fonts/fonts.hpp"
#include "lib/functions.hpp"
#include "tasks/led_handler.hpp"

typedef struct {
  uint dispNum;
  bool partial;
} UpdateEpdData;

void setupDisplays();
void initDisplays();
void taskEpd(void *pvParameters);
void resetAllDisplays();
void resetSingleDisplay(int i);

std::array<String, 7> getCurrentEpdContent();

void setEpdContent(std::array<String, 7> newEpdContent);
void splitText(const uint dispNum, String top, String bottom, bool partial);
void showDigit(const uint dispNum, char chr, bool partial, const GFXfont *font);

void refreshDisplay(void *pvParameters);
void fullRefresh(void *pvParameters);
void updateDisplay(void *pvParameters);
//void genQrCode(String text, uint8_t *qrcode[qrcodegen_BUFFER_LEN_MAX]);

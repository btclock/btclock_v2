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
#ifdef USE_UNIVERSAL_PIN
#include <native_pin.hpp>
#include <mcp23x17_pin.hpp>
#include <universal_pin.hpp>
#endif

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

std::array<String, NUM_SCREENS> getCurrentEpdContent();

void resetAllDisplays();
void resetSingleDisplay(int i);
void setEpdContent(std::array<String, NUM_SCREENS> newEpdContent);
void splitText(const uint dispNum, String top, String bottom, bool partial);
void showDigit(const uint dispNum, char chr, bool partial, const GFXfont *font);

void refreshDisplay(void *pvParameters);
void fullRefresh(void *pvParameters);
extern "C" void updateDisplay(void *pvParameters) noexcept;
//void genQrCode(String text, uint8_t *qrcode[qrcodegen_BUFFER_LEN_MAX]);

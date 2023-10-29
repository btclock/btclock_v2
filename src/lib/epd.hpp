#pragma once

#include "config.h"
#include "shared.hpp"
#include "qrcodegen.h"

#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#ifdef IS_BW
#include <GxEPD2_BW.h>
#else 
#include <GxEPD2_3C.h>
#endif

int getBgColor();
int getFgColor();
void setBgColor(int color);
void setFgColor(int color);

void showSetupQr(const String& ssid, const String& password);

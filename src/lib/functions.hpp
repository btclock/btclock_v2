#include <ESP32Time.h>
#include <WiFi.h>
#include <Arduino.h>
#include <WiFiManager.h>

#include "config.h"
#include "shared.hpp"
#include "Adafruit_GFX.h"
#include "tasks/blocknotify.hpp"
#include "tasks/minute.hpp"
#include "tasks/get_price.hpp"
#include "tasks/epd.hpp"

void setupComponents();
void setupWifi();
void synchronizeTime();
void setupPreferences();
void setLights(int r, int g, int b);

uint getCurrentScreen();
void setCurrentScreen(uint screen);
void handleScreenTasks(uint screen);

void timebasedChangeTask(void *parameter);

GFXfont getFont(uint size);
int getBgColor();
int getFgColor();

void setupSoftAP();
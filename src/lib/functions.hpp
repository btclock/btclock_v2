#include <ESP32Time.h>
#include <WiFi.h>
#include <Arduino.h>
#include <WiFiManager.h>
#include <WiFiMulti.h>

#include "config.h"
#include "shared.hpp"
#include "Adafruit_GFX.h"
#include "lib/epd.hpp"
#include "tasks/blocknotify.hpp"
#include "tasks/minute.hpp"
#include "tasks/get_price.hpp"
#include "tasks/epd.hpp"

void setupComponents();
void setupWifi();
void synchronizeTime();
void setupPreferences();
void setLights(int r, int g, int b);
void flashTemporaryLights(int r, int g, int b);
void setupI2C();
void onI2CReceive(int len);
void onI2CRequest();

uint getCurrentScreen();
void setCurrentScreen(uint screen);
void handleScreenTasks(uint screen);
void showNetworkSettings();
void toggleScreenTimer();

void timebasedChangeTask(void *parameter);

GFXfont getFont(uint size);

void setupSoftAP();
void nextScreen();
void previousScreen();
int modulo(int x,int N);
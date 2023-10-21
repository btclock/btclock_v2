#include "SPIFFS.h"
#include <WebServer.h>
#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>
#include "AsyncJson.h"

#include <ESPmDNS.h>
#include "lib/functions.hpp"
#include "lib/epd.hpp"

#include <string>
#include "tasks/epd.hpp"
#include "shared.hpp"

void setupWebserver();
void onApiStatus(AsyncWebServerRequest *request);
void onApiSystemStatus(AsyncWebServerRequest *request);
void onApiActionPause(AsyncWebServerRequest *request);
void onApiActionTimerRestart(AsyncWebServerRequest *request);
void onApiActionUpdate(AsyncWebServerRequest *request);
void onApiSettingsGet(AsyncWebServerRequest *request);
void onApiSettingsPost(AsyncWebServerRequest *request);
void onApiShowScreen(AsyncWebServerRequest *request);
void onApiShowText(AsyncWebServerRequest *request);
void onApiShowTextAdvanced(AsyncWebServerRequest *request, JsonVariant &json);

void onApiFullRefresh(AsyncWebServerRequest *request);
void onApiCountdown(AsyncWebServerRequest *request);
void onApiWifiScan(AsyncWebServerRequest *request);
void onApiRestart(AsyncWebServerRequest *request);

void onApiLightsOff(AsyncWebServerRequest *request);
void onApiLightsFlash(AsyncWebServerRequest *request);
void onApiLightsSetColor(AsyncWebServerRequest *request);

void onIndex(AsyncWebServerRequest *request);
void onNotFound(AsyncWebServerRequest *request);

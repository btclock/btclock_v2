#include "webserver.hpp"
#include "webserver/OneParamRewrite.hpp"
#include <string>

AsyncWebServer server(80);

void setupWebserver()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println(F("An Error has occurred while mounting SPIFFS"));
        return;
    }

    server.serveStatic("/css", SPIFFS, "/css/");
    server.serveStatic("/js", SPIFFS, "/js/");
    server.serveStatic("/font", SPIFFS, "/font/");

    server.on("/", HTTP_GET, onIndex);
    server.on("/api/action/pause", HTTP_GET, onApiActionPause);
    server.on("/api/action/timer_restart", HTTP_GET, onApiActionTimerRestart);
    server.on("/api/action/update", HTTP_GET, onApiActionUpdate);
    server.on("/api/full_refresh", HTTP_GET, onApiFullRefresh);
    server.on("/api/status", HTTP_GET, onApiStatus);
    server.on("/api/system_status", HTTP_GET, onApiSystemStatus);

    server.on("/api/settings", HTTP_GET, onApiSettingsGet);
    server.on("/api/settings", HTTP_POST, onApiSettingsPost);
    server.on("/api/show/screen", HTTP_GET, onApiShowScreen);
    server.on("/api/show/text", HTTP_GET, onApiShowText);
    // server.on("/api/show/custom", HTTP_GET, onApiShowTextAdvanced);
    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/api/show/custom", onApiShowTextAdvanced);
    server.addHandler(handler);

    server.on("/api/restart", HTTP_GET, onApiRestart);

    server.on("/api/countdown", HTTP_GET, onApiCountdown);

    server.on("/api/lights/off", HTTP_GET, onApiLightsOff);
    server.on("/api/lights/flash", HTTP_GET, onApiLightsFlash);
    server.on("/api/lights/color", HTTP_GET, onApiLightsSetColor);
    server.on("^\\/api\\/lights\\/([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$", HTTP_GET, onApiLightsSetColor);
    server.addRewrite(new OneParamRewrite("/api/show/screen/{s}", "/api/show/screen?s={s}"));
    server.addRewrite(new OneParamRewrite("/api/show/text/{text}", "/api/show/text?t={text}"));
    server.addRewrite(new OneParamRewrite("/api/show/number/{number}", "/api/show/text?t={text}"));
    // server.addRewrite(new OneParamRewrite("/api/lights/{color}", "/api/lights/color?c={color}"));

    server.onNotFound(onNotFound);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

    // Start server
    server.begin();
    if (!MDNS.begin(HOSTNAME))
    {
        Serial.println(F("Error setting up MDNS responder!"));
        while (1)
        {
            delay(1000);
        }
    }
    MDNS.addService("http", "tcp", 80);
    Serial.println(F("Webserver should be running"));
}

/**
 * @Api
 * @Path("/api/status")
*/
void onApiStatus(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    StaticJsonDocument<512> root;
    root["currentScreen"] = String(getCurrentScreen());
    root["timerRunning"] = timerRunning;
    root["numScreens"] = NUM_SCREENS;
    JsonArray data = root.createNestedArray("data");
    JsonArray rendered = root.createNestedArray("rendered");
    String epdContent[7];

#ifdef WITH_RGB_LED

    uint32_t pixelColors[NEOPIXEL_COUNT];
    JsonArray ledStatus = root.createNestedArray("ledStatus");

    for (int i = 0; i < NEOPIXEL_COUNT; i++)
    {
        pixelColors[i] = pixels.getPixelColor(i);
    }

    copyArray(pixelColors, ledStatus);
#endif

    std::copy(std::begin(getCurrentEpdContent()), std::end(getCurrentEpdContent()), epdContent);

    copyArray(epdContent, data);
    copyArray(epdContent, rendered);
    serializeJson(root, *response);

    request->send(response);
}

/**
 * @Api
 * @Path("/api/action/pause")
*/
void onApiActionPause(AsyncWebServerRequest *request)
{
    timerRunning = false;
    Serial.println(F("Update timer paused"));

    request->send(200);
};

/**
 * @Api
 * @Path("/api/action/full_refresh")
*/
void onApiFullRefresh(AsyncWebServerRequest *request)
{

    xTaskCreate(fullRefresh, "Full Refresh", 10000, NULL, 5, NULL); // create task

    request->send(200);
}

/**
 * @Api
 * @Path("/api/action/timer_restart")
*/
void onApiActionTimerRestart(AsyncWebServerRequest *request)
{
    // moment = millis();
    timerRunning = true;
    Serial.println(F("Update timer restarted"));

    request->send(200);
}

/**
 * @Api
 * @Path("/api/action/update")
 * @Parameter int rate Time in minutes
*/
void onApiActionUpdate(AsyncWebServerRequest *request)
{
    if (request->hasParam("rate"))
    {
        AsyncWebParameter *p = request->getParam("rate");
        timerSeconds = p->value().toInt() * 60;
        preferences.putUInt("timerSeconds", timerSeconds);
    }

    timerRunning = true;

    request->send(200);
}

/**
 * @Api
 * @Method GET
 * @Path("/api/settings")
*/
void onApiSettingsGet(AsyncWebServerRequest *request)
{
    StaticJsonDocument<768> root;
    root["fgColor"] = getFgColor();
    root["bgColor"] = getBgColor();
    root["timerSeconds"] = timerSeconds;
    root["timerRunning"] = timerRunning;
    root["fullRefreshMin"] = preferences.getUInt("fullRefreshMin", 30);
    root["wpTimeout"] = preferences.getUInt("wpTimeout", 600);
    root["tzOffset"] = preferences.getInt("gmtOffset", TIME_OFFSET_SECONDS) / 60;
    root["useBitcoinNode"] = preferences.getBool("useNode", false);
    root["rpcPort"] = preferences.getUInt("rpcPort", BITCOIND_PORT);
    root["rpcUser"] = preferences.getString("rpcUser", BITCOIND_RPC_USER);
    root["rpcHost"] = preferences.getString("rpcHost", BITCOIND_HOST);
    root["mempoolInstance"] = preferences.getString("mempoolInstance", DEFAULT_MEMPOOL_INSTANCE);

#ifdef IS_BW
    root["epdColors"] = 2;
#else
    root["epdColors"] = 3;
#endif
    root["ledFlashOnUpdate"] = preferences.getBool("ledFlashOnUpd", false);
    root["ledBrightness"] = preferences.getUInt("ledBrightness", 128);

#ifdef GIT_REV
    root["gitRev"] = String(GIT_REV);
#endif
#ifdef LAST_BUILD_TIME
    root["lastBuildTime"] = String(LAST_BUILD_TIME);
#endif
    JsonArray screens = root.createNestedArray("screens");

    for (int i = 0; i < screenNameMap.size(); i++)
    {
        JsonObject o = screens.createNestedObject();
        String key = "screen" + String(i) + "Visible";
        o["id"] = i;
        o["name"] = screenNameMap[i];
        o["enabled"] = preferences.getBool(key.c_str(), true);
    }

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(root, *response);
    
    request->send(response);
}

bool processEpdColorSettings(AsyncWebServerRequest *request)
{
    bool settingsChanged = false;
    if (request->hasParam("fgColor", true))
    {
        AsyncWebParameter *fgColor = request->getParam("fgColor", true);
        preferences.putUInt("fgColor", strtol(fgColor->value().c_str(), NULL, 16));
        setFgColor(int(strtol(fgColor->value().c_str(), NULL, 16)));
        Serial.print(F("Setting foreground color to "));
        Serial.println(fgColor->value().c_str());
        settingsChanged = true;
    }
    if (request->hasParam("bgColor", true))
    {
        AsyncWebParameter *bgColor = request->getParam("bgColor", true);

        preferences.putUInt("bgColor", strtol(bgColor->value().c_str(), NULL, 16));
        setBgColor(int(strtol(bgColor->value().c_str(), NULL, 16)));
        Serial.print(F("Setting background color to "));
        Serial.println(bgColor->value().c_str());
        settingsChanged = true;
    }

    return settingsChanged;
}

void onApiEpdSettingsPost(AsyncWebServerRequest *request)
{
    bool settingsChanged = false;

    settingsChanged = processEpdColorSettings(request);

    request->send(200);
    if (settingsChanged)
    {
        flashTemporaryLights(0, 255, 0);

        Serial.println(F("Settings changed"));
    }
}

void onApiSettingsPost(AsyncWebServerRequest *request)
{
    int params = request->params();
    bool settingsChanged = false;

    settingsChanged = processEpdColorSettings(request);

    if (request->hasParam("ledFlashOnUpd", true))
    {
        AsyncWebParameter *ledFlashOnUpdate = request->getParam("ledFlashOnUpd", true);

        preferences.putBool("ledFlashOnUpd", ledFlashOnUpdate->value().toInt());
        Serial.print("Setting led flash on update to ");
        Serial.println(ledFlashOnUpdate->value().c_str());
        settingsChanged = true;
    }
    else
    {
        preferences.putBool("ledFlashOnUpd", 0);
        Serial.print("Setting led flash on update to false");
        settingsChanged = true;
    }

    if (request->hasParam("mempoolInstance", true))
    {
        AsyncWebParameter *mempoolInstance = request->getParam("mempoolInstance", true);

        preferences.putString("mempoolInstance", mempoolInstance->value().c_str());
        Serial.print("Setting mempool instance to ");
        Serial.println(mempoolInstance->value().c_str());
        settingsChanged = true;
    }

    if (request->hasParam("ledBrightness", true))
    {
        AsyncWebParameter *ledBrightness = request->getParam("ledBrightness", true);

        preferences.putUInt("ledBrightness", ledBrightness->value().toInt());
        Serial.print("Setting brightness to ");
        Serial.println(ledBrightness->value().c_str());
        settingsChanged = true;
    }

    if (request->hasParam("fullRefreshMin", true))
    {
        AsyncWebParameter *fullRefreshMin = request->getParam("fullRefreshMin", true);

        preferences.putUInt("fullRefreshMin", fullRefreshMin->value().toInt());
        Serial.print("Set full refresh minutes to ");
        Serial.println(fullRefreshMin->value().c_str());
        settingsChanged = true;
    }

    if (request->hasParam("wpTimeout", true))
    {
        AsyncWebParameter *wpTimeout = request->getParam("wpTimeout", true);

        preferences.putUInt("wpTimeout", wpTimeout->value().toInt());
        Serial.print("Set WiFi portal timeout seconds to ");
        Serial.println(wpTimeout->value().c_str());
        settingsChanged = true;
    }

    for (int i = 0; i < screenNameMap.size(); i++)
    {
        String key = "screen[" + String(i) + "]";
        String prefKey = "screen" + String(i) + "Visible";
        bool visible = false;
        if (request->hasParam(key, true))
        {
            AsyncWebParameter *screenParam = request->getParam(key, true);
            visible = screenParam->value().toInt();
        }
        Serial.print("Setting screen " + String(i) + " to ");
        Serial.println(visible);

        preferences.putBool(prefKey.c_str(), visible);
    }

    if (request->hasParam("tzOffset", true))
    {
        AsyncWebParameter *p = request->getParam("tzOffset", true);
        int tzOffsetSeconds = p->value().toInt() * 60;
        preferences.putInt("gmtOffset", tzOffsetSeconds);
        Serial.print("Setting tz offset to ");
        Serial.println(tzOffsetSeconds);
        settingsChanged = true;
    }

    if (request->hasParam("timePerScreen", true))
    {
        AsyncWebParameter *p = request->getParam("timePerScreen", true);
        timerSeconds = p->value().toInt() * 60;
        preferences.putUInt("timerSeconds", timerSeconds);
        settingsChanged = true;
    }

    if (request->hasParam("useBitcoinNode", true))
    {
        AsyncWebParameter *p = request->getParam("useBitcoinNode", true);
        bool useBitcoinNode = p->value().toInt();
        preferences.putBool("useNode", useBitcoinNode);
        settingsChanged = true;

        String rpcVars[] = {"rpcHost", "rpcPort", "rpcUser", "rpcPass"};

        for (String v : rpcVars)
        {
            if (request->hasParam(v, true))
            {
                AsyncWebParameter *pv = request->getParam(v, true);
                // Don't store an empty password, probably new settings save
                if (!(v.equals("rpcPass") && pv->value().length() == 0))
                {
                    preferences.putString(v.c_str(), pv->value().c_str());
                }
            }
        }
    }
    else
    {
        preferences.putBool("useNode", false);
        settingsChanged = true;
    }

    request->send(200);
    if (settingsChanged)
    {
        flashTemporaryLights(0, 255, 0);

        Serial.println(F("Settings changed"));
    }
}

void onApiShowScreen(AsyncWebServerRequest *request)
{
    if (request->hasParam("s"))
    {
        AsyncWebParameter *p = request->getParam("s");
        uint currentScreen = p->value().toInt();
        setCurrentScreen(currentScreen);
    }
    request->send(200);
}

void onApiShowTextAdvanced(AsyncWebServerRequest *request, JsonVariant &json)
{
    //    JsonObject& jsonObj = json.as<JsonObject>();
    // serializeJson(json, Serial);

    JsonArray screens = json.as<JsonArray>();

    std::array<String, 7> epdContent;
    int i = 0;
    for (JsonVariant s : screens)
    {
        epdContent[i] = s.as<String>();
        Serial.println(s.as<String>());
        i++;
    }

    CustomTextScreen::setText(epdContent);

    setCurrentScreen(SCREEN_CUSTOM);
    request->send(200);
}

void onApiShowText(AsyncWebServerRequest *request)
{
    if (request->hasParam("t"))
    {
        AsyncWebParameter *p = request->getParam("t");
        String t = p->value();
        t.toUpperCase(); // This is needed as long as lowercase letters are glitchy
        CustomTextScreen::setSimpleText(t);
    }
    setCurrentScreen(SCREEN_CUSTOM);
    request->send(200);
}

void onApiRestart(AsyncWebServerRequest *request)
{
    request->send(200);
    esp_restart();
}

// void onApiWifiScan(AsyncWebServerRequest *request)
// {
//     WiFi.scanNetworks(true);

//     int n = WiFi.scanComplete();

//     DynamicJsonDocument doc(1024);

//     for (int i = 0; i < n; ++i)
//     {
//         JsonObject wifi = doc.createNestedObject(WiFi.BSSID(i));
//         Serial.println(WiFi.SSID(i));
//         wifi["ssid"] = WiFi.SSID(i);
//         wifi["rssi"] = WiFi.RSSI(i);
//         wifi["encryptionType"] = WiFi.encryptionType(i);
//     }

//     String responseText;
//     serializeJson(doc, responseText);
//     request->send(200, "application/json", responseText);
// }

void onApiCountdown(AsyncWebServerRequest *request)
{
    if (request->hasParam("s"))
    {
        AsyncWebParameter *p = request->getParam("s");
        uint seconds = p->value().toInt();
        CountdownScreen::setCountdownSeconds(seconds);
    }
    timerRunning = false;
    setCurrentScreen(SCREEN_COUNTDOWN);
    xTaskCreate(CountdownScreen::countdownTask, "Countdown", 4096, NULL, 50, NULL);
    request->send(200);
}

void onIndex(AsyncWebServerRequest *request) { request->send(SPIFFS, "/index.html", String(), false); }
void onNotFound(AsyncWebServerRequest *request)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    else
    {
        request->send(404);
    }
};

void onApiLightsOff(AsyncWebServerRequest *request)
{
    setLights(0, 0, 0);
    request->send(200);
}

void onApiLightsFlash(AsyncWebServerRequest *request)
{
    setLights(255, 0, 0);
    request->send(200);
}

void onApiLightsSetColor(AsyncWebServerRequest *request)
{
    String rgbColor = request->pathArg(0);
    uint r, g, b;
    sscanf(rgbColor.c_str(), "%02x%02x%02x", &r, &g, &b);
    setLights(r, g, b);
    request->send(200, "text/plain", rgbColor);
}

void onApiSystemStatus(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");

    StaticJsonDocument<128> root;

    root["espFreeHeap"] = ESP.getFreeHeap();
    root["espHeapSize"] = ESP.getHeapSize();
    root["espFreePsram"] = ESP.getFreePsram();
    root["espPsramSize"] = ESP.getPsramSize();

    serializeJson(root, *response);

    request->send(response);
}
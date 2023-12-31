#include "functions.hpp"

Preferences preferences;
uint timerSeconds;
uint currentScreen;
std::map<int, std::string> screenNameMap;

#ifndef NO_MCP
Adafruit_MCP23X17 mcp;
const char MCP_INT_PIN = 8;

#endif
bool timerRunning = true;

uint wifiConnectionLostCount = 0;

#ifdef WITH_RGB_LED
#ifndef NEOPIXEL_PIN
#define NEOPIXEL_PIN 34
#endif
#ifndef NEOPIXEL_COUNT
#define NEOPIXEL_COUNT 3
#endif

Adafruit_NeoPixel pixels(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
#endif

String softAP_SSID;
String softAP_password;
//WiFiMulti wifiMulti;

WiFiManager wm;
bool screenVisible[5];

void setupSoftAP()
{
    byte mac[6];
    WiFi.macAddress(mac);
    softAP_SSID = getMyHostname().c_str();
    WiFi.setHostname(getMyHostname().c_str());
    softAP_password = base64::encode(String(mac[2], 16) + String(mac[4], 16) + String(mac[5], 16) + String(mac[1], 16)).substring(2, 10);
}

void setupComponents()
{
    if (psramInit())
    {
        Serial.println(F("PSRAM is correctly initialized"));
    }
    else
    {
        Serial.println(F("PSRAM not available"));
    }
#ifdef WITH_RGB_LED
    pixels.begin();
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 255, 0));
    pixels.setPixelColor(2, pixels.Color(0, 0, 255));
    pixels.setPixelColor(3, pixels.Color(255, 255, 255));
    pixels.show();
#endif

    // delay(3000);
    // Serial.println(F("Leds should be on"));

#ifndef NO_MCP
    if (!mcp.begin_I2C(0x20))
    {
        Serial.println(F("Error MCP23017"));
        pixels.setPixelColor(0, pixels.Color(255, 0, 0));
        pixels.setPixelColor(1, pixels.Color(255, 0, 0));
        pixels.setPixelColor(2, pixels.Color(255, 0, 0));
        pixels.setPixelColor(3, pixels.Color(255, 0, 0));
        pixels.show();
        while (1)
            ;
    }
    else
    {
        Serial.println(F("MCP23017 ok"));
        pixels.setPixelColor(0, pixels.Color(0, 255, 0));
        pixels.setPixelColor(1, pixels.Color(0, 255, 0));
        pixels.setPixelColor(2, pixels.Color(0, 255, 0));
        pixels.setPixelColor(3, pixels.Color(0, 255, 0));
        pixels.show();
        //  delay(200);
        pinMode(MCP_INT_PIN, INPUT);
        mcp.setupInterrupts(false, false, LOW);
    }
#endif

#ifdef WITH_BUTTONS
    for (int i = 0; i < 4; i++)
    {
        mcp.pinMode(i, INPUT_PULLUP);
        mcp.setupInterruptPin(i, LOW);
    }
#endif
}

void synchronizeTime()
{
    configTime(preferences.getInt("gmtOffset", TIME_OFFSET_SECONDS), 0, NTP_SERVER);
    struct tm timeinfo;

    while (!getLocalTime(&timeinfo))
    {
        configTime(preferences.getInt("gmtOffset", TIME_OFFSET_SECONDS), 0, NTP_SERVER);
        delay(500);
        Serial.println(F("Retry set time"));
    }

    rtc.setTimeStruct(timeinfo);
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setupWifi()
{
#ifndef NO_MCP
    if (mcp.digitalRead(3) == LOW)
    {
        pixels.setPixelColor(0, pixels.Color(0, 0, 255));
        pixels.setPixelColor(1, pixels.Color(0, 0, 255));
        pixels.setPixelColor(2, pixels.Color(0, 0, 255));
        pixels.setPixelColor(3, pixels.Color(0, 0, 255));
        pixels.show();

        delay(1500);

        if (mcp.digitalRead(3) == LOW)
        {
            pixels.setPixelColor(0, pixels.Color(255, 0, 0));
            pixels.setPixelColor(1, pixels.Color(0, 0, 255));
            pixels.setPixelColor(2, pixels.Color(255, 0, 0));
            pixels.setPixelColor(3, pixels.Color(0, 0, 255));
            pixels.show();
            Serial.println(F("Erasing WiFi Config, restarting"));
            wm.resetSettings();
            ESP.restart();
        }

        return;
    }
#endif

    //WiFi.persistent(true);
    WiFi.enableLongRange(false);
    setupSoftAP();

    wm.setConfigPortalTimeout(preferences.getUInt("wpTimeout", 600));
    wm.setWiFiAutoReconnect(true);
    wm.setAPCallback([&](WiFiManager *wifiManager)
                     {
    showSetupQr(softAP_SSID, softAP_password); 
    Serial.printf("Entered config mode:ip=%s, ssid='%s', pass='%s'\n", 
      WiFi.softAPIP().toString().c_str(), 
      wifiManager->getConfigPortalSSID().c_str(), 
      softAP_password.c_str()); });

    bool ac = wm.autoConnect(softAP_SSID.c_str(), softAP_password.c_str());
    WiFi.onEvent(onWifiLostIp, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_LOST_IP);

}

void onWifiLostIp(WiFiEvent_t event, WiFiEventInfo_t info) {
    ESP.restart();
}

void setupPreferences()
{
    preferences.begin("btclock", false);
    timerSeconds = preferences.getUInt("timerSeconds", 1800);
    currentScreen = preferences.getUInt("currentScreen", 0);
    // handleScreenTasks(currentScreen);
    setFgColor(preferences.getUInt("fgColor", DEFAULT_FG_COLOR));
    setBgColor(preferences.getUInt("bgColor", DEFAULT_BG_COLOR));
    preferences.getBool("ledFlashOnUpd", false);

    screenNameMap = {{SCREEN_BLOCK_HEIGHT, "Block Height"},
                     {SCREEN_MSCW_TIME, "Sats per dollar"},
                     {SCREEN_BTC_TICKER, "Ticker"},
                     {SCREEN_TIME, "Time"},
                     {SCREEN_HALVING_COUNTDOWN, "Halving countdown"},
                     {SCREEN_MARKET_CAP, "Market Cap"}
                     };

#ifdef WITH_RGB_LED
    pixels.setBrightness(preferences.getUInt("ledBrightness", 128));
    pixels.setPixelColor(3, pixels.Color(255, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 255, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 255));
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));
    pixels.show();
#endif

    for (int i = 0; i < screenNameMap.size(); i++)
    {
        String key = "screen" + String(i) + "Visible";
        screenVisible[i] = preferences.getBool(key.c_str(), true); // Default to true if not set
    }

    xTaskCreate(timebasedChangeTask, "tbc", 5000, NULL, 15, NULL);
}

uint getCurrentScreen()
{
    return currentScreen;
}

void setCurrentScreen(uint screen)
{
    if (screen != SCREEN_CUSTOM)
    {
        preferences.putUInt("currentScreen", screen);
    }

    currentScreen = screen;
    handleScreenTasks(screen);
}

void handleScreenTasks(uint screen)
{
    if (blockNotifyTaskHandle)
        vTaskSuspend(blockNotifyTaskHandle);
    if (getPriceTaskHandle)
        vTaskSuspend(getPriceTaskHandle);
    switch (currentScreen)
    {
    case SCREEN_BLOCK_HEIGHT:
    case SCREEN_HALVING_COUNTDOWN:
        if (blockNotifyTaskHandle)
            vTaskResume(blockNotifyTaskHandle);
        break;
    case SCREEN_BTC_TICKER:
    case SCREEN_MSCW_TIME:
        if (getPriceTaskHandle)
            vTaskResume(getPriceTaskHandle);
        break;
    case SCREEN_MARKET_CAP:
        if (getPriceTaskHandle)
            vTaskResume(getPriceTaskHandle);
        if (blockNotifyTaskHandle)
            vTaskResume(blockNotifyTaskHandle);
        break;
    case SCREEN_TIME:
        if (minuteTaskHandle)
        {
            TimeScreen::onActivate();
            vTaskResume(minuteTaskHandle);
        }
        break;
    }
}

void toggleScreenTimer()
{
    timerRunning = !timerRunning;

    if (!timerRunning)
    {
        Serial.println(F("Stopping screen timer..."));
        for (int i = NEOPIXEL_COUNT; i >= 0; i--)
        {
            for (int j = NEOPIXEL_COUNT; j >= 0; j--)
            {
                uint32_t c = pixels.Color(0, 0, 0);
                if (i == j)
                    c = pixels.Color(0, 255, 0);
                pixels.setPixelColor(j, c);
            }

            pixels.show();

            delay(100);
        }

        delay(900);

        pixels.clear();
        pixels.show();
    }
    else
    {
        Serial.println(F("Starting screen timer..."));

        pixels.setPixelColor(3, pixels.Color(0, 255, 0));
        pixels.setPixelColor(2, pixels.Color(0, 0, 0));
        pixels.setPixelColor(1, pixels.Color(0, 0, 0));
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();

        delay(1000);

        for (int i = NEOPIXEL_COUNT; i--; i > 0)
        {

            for (int j = NEOPIXEL_COUNT; j--; j > 0)
            {
                uint32_t c = pixels.Color(0, 0, 0);
                if (i == j)
                    c = pixels.Color(0, 255, 0);

                pixels.setPixelColor(j, c);
            }

            pixels.show();

            delay(100);
        }

        pixels.clear();
        pixels.show();
    }
}

void timebasedChangeTask(void *parameter)
{
    uint32_t moment = millis();

    for (;;)
    {
        if (millis() - moment > timerSeconds * 1000 && timerRunning)
        {
            int newCurrentScreen = (getCurrentScreen() + 1) % screenCount;
            String key = "screen" + String(newCurrentScreen) + "Visible";

            while (!preferences.getBool(key.c_str(), true))
            {
                newCurrentScreen = (newCurrentScreen + 1) % screenCount;
                key = "screen" + String(newCurrentScreen) + "Visible";
            }
            setCurrentScreen(newCurrentScreen);
            moment = millis();
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int modulo(int x, int N)
{
    return (x % N + N) % N;
}

void nextScreen()
{
    int newCurrentScreen = (getCurrentScreen() + 1) % screenCount;
    String key = "screen" + String(newCurrentScreen) + "Visible";

    while (!preferences.getBool(key.c_str(), true))
    {
        newCurrentScreen = (newCurrentScreen + 1) % screenCount;
        key = "screen" + String(newCurrentScreen) + "Visible";
    }
    setCurrentScreen(newCurrentScreen);
}

void previousScreen()
{
    int newCurrentScreen = modulo(getCurrentScreen() - 1, screenCount);
    String key = "screen" + String(newCurrentScreen) + "Visible";

    while (!preferences.getBool(key.c_str(), true))
    {
        newCurrentScreen = modulo(newCurrentScreen - 1, screenCount);
        key = "screen" + String(newCurrentScreen) + "Visible";
    }
    setCurrentScreen(newCurrentScreen);
}

void showNetworkSettings()
{
    std::array<String, NUM_SCREENS> epdContent = {"", "", "", "", "", "", ""};

    String ipAddr = WiFi.localIP().toString();
    String subNet = WiFi.subnetMask().toString();

    epdContent[0] = "IP/Subnet";

    int ipAddrPos = 0;
    int subnetPos = 0;
    for (int i = 0; i < 4; i++)
    {
        epdContent[1 + i] = ipAddr.substring(0, ipAddr.indexOf('.')) + "/" + subNet.substring(0, subNet.indexOf('.'));
        ipAddrPos = ipAddr.indexOf('.') + 1;
        subnetPos = subNet.indexOf('.') + 1;
        ipAddr = ipAddr.substring(ipAddrPos);
        subNet = subNet.substring(subnetPos);
    }
    epdContent[NUM_SCREENS-2] = "RAM/Status";

    epdContent[NUM_SCREENS-1] = String((int)round(ESP.getFreeHeap()/1024)) + "/" + (int)round(ESP.getHeapSize()/1024);

    CustomTextScreen::setText(epdContent);

    setCurrentScreen(SCREEN_CUSTOM);
}

void setLights(int r, int g, int b)
{
#ifdef WITH_RGB_LED
    for (int i = 0; i < NEOPIXEL_COUNT; i++)
    {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
    }

    pixels.show();
#endif
}

void flashTemporaryLights(int r, int g, int b)
{
#ifdef WITH_RGB_LED

    uint32_t oldLights[NEOPIXEL_COUNT];

    // get current state
    for (int i = 0; i < NEOPIXEL_COUNT; i++)
    {
        oldLights[i] = pixels.getPixelColor(i);
    }

    // flash three times in given color
    for (int t = 0; t < 3; t++)
    {
        for (int i = 0; i < NEOPIXEL_COUNT; i++)
        {
            pixels.setPixelColor(i, pixels.Color(r, g, b));
        }

        pixels.show();
        delay(200);

        pixels.clear();
        pixels.show();
        delay(200);
    }

    // revert to previous state
    for (int i = 0; i < NEOPIXEL_COUNT; i++)
    {
        pixels.setPixelColor(i, oldLights[i]);
    }

    pixels.show();
#endif
}

void setupI2C()
{
    bool slaveMode = preferences.getBool("I2CSlaveMode", false);

    if (slaveMode)
    {
        Serial.println(F("I2C Slave Mode enabled"));
        Wire.onReceive(onI2CReceive);
        Wire.begin((uint8_t)I2C_DEV_ADDR);
    }
}

void onI2CReceive(int len)
{
    Serial.printf("onReceive[%d]: ", len);
    while (Wire.available())
    {
        Serial.write(Wire.read());
    }
    Serial.println();
}

void onI2CRequest()
{
    Wire.print("I2C Packets.");
    Serial.println(F("onRequest"));
}

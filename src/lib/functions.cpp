#include "functions.hpp"

Preferences preferences;
uint timerSeconds;
uint currentScreen;
std::map<int, std::string> screenNameMap;

#ifndef NO_MCP
Adafruit_MCP23X17 mcp;
const int MCP_INT_PIN = 8;

#endif
bool timerRunning = true;
int fgColor;
int bgColor;
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
WiFiManager wm;
bool screenVisible[5];

void setupSoftAP()
{
    byte mac[6];
    WiFi.macAddress(mac);
    softAP_SSID = String("BTClock" + String(mac[3], 16) + String(mac[1], 16));
    softAP_password = base64::encode(String(mac[2]) + String(mac[4]) + String(mac[5])).substring(2, 12);
}

void setupComponents()
{
#ifndef NO_MCP
    if (!mcp.begin_I2C())
    {
        Serial.println("Error MCP23017");
        while (1)
            ;
    }
    else
    {
        Serial.println("MCP23017 ok");
        pinMode(MCP_INT_PIN, INPUT);
        mcp.setupInterrupts(true, false, LOW);
    }
#endif

#ifdef WITH_RGB_LED
    pixels.begin();
    pixels.show();
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
    configTime(preferences.getUInt("gmtOffset", TIME_OFFSET_SECONDS), 0, NTP_SERVER);
    struct tm timeinfo;

    while (!getLocalTime(&timeinfo))
    {
        configTime(preferences.getUInt("gmtOffset", TIME_OFFSET_SECONDS), 0, NTP_SERVER);
        delay(500);
        Serial.println("Retry set time");
    }

    rtc.setTimeStruct(timeinfo);
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setupWifi()
{
    setupSoftAP();
    wm.setAPCallback([&](WiFiManager *wifiManager)
                     {
    showSetupQr(softAP_SSID, softAP_password); 
    Serial.printf("Entered config mode:ip=%s, ssid='%s', pass='%s'\n", 
      WiFi.softAPIP().toString().c_str(), 
      wifiManager->getConfigPortalSSID().c_str(), 
      softAP_password.c_str()); });

    bool ac = wm.autoConnect(softAP_SSID.c_str(), softAP_password.c_str());
}

void setupPreferences()
{
    preferences.begin("btclock", false);
    timerSeconds = preferences.getUInt("timerSeconds", 1800);
    currentScreen = preferences.getUInt("currentScreen", 0);
    // handleScreenTasks(currentScreen);
    fgColor = preferences.getUInt("fgColor", DEFAULT_FG_COLOR);
    bgColor = preferences.getUInt("bgColor", DEFAULT_BG_COLOR);
    preferences.getBool("ledFlashOnUpd", false);

    screenNameMap = {{SCREEN_BLOCK_HEIGHT, "Block Height"},
                     {SCREEN_MSCW_TIME, "Sats per dollar"},
                     {SCREEN_BTC_TICKER, "Ticker"},
                     {SCREEN_TIME, "Time"},
                     {SCREEN_HALVING_COUNTDOWN, "Halving countdown"}};

#ifdef WITH_RGB_LED
    pixels.setBrightness(preferences.getUInt("ledBrightness", 128));
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 255, 0));
    pixels.setPixelColor(2, pixels.Color(0, 0, 255));
    pixels.setPixelColor(3, pixels.Color(255, 255, 255));
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
    preferences.putUInt("currentScreen", screen);

    currentScreen = screen;
    handleScreenTasks(screen);
}

void handleScreenTasks(uint screen)
{
    if (blockNotifyTaskHandle)
        vTaskSuspend(blockNotifyTaskHandle);
    if (getPriceTaskHandle)
        vTaskSuspend(getPriceTaskHandle);
    if (minuteTaskHandle)
        vTaskSuspend(minuteTaskHandle);
    switch (currentScreen)
    {
    case SCREEN_BLOCK_HEIGHT:
        if (blockNotifyTaskHandle)
        {
            vTaskResume(blockNotifyTaskHandle);
        }
        break;
    case SCREEN_HALVING_COUNTDOWN:
        if (blockNotifyTaskHandle)
            vTaskResume(blockNotifyTaskHandle);
        break;
    case SCREEN_BTC_TICKER:
        if (getPriceTaskHandle)
            vTaskResume(getPriceTaskHandle);
        break;
    case SCREEN_MSCW_TIME:
        if (getPriceTaskHandle)
            vTaskResume(getPriceTaskHandle);
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

int getBgColor()
{
    return bgColor;
}

int getFgColor()
{
    return fgColor;
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

void setupI2C()
{
    bool slaveMode = preferences.getBool("I2CSlaveMode", false);

    if (slaveMode)
    {
        Serial.println("I2C Slave Mode enabled");
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
    Serial.println("onRequest");
}
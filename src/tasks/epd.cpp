#include "epd.hpp"

#ifdef IS_S3
// reversed
const int EPD_CS[7] = {17, 21, 33, 10, 6, 4, 2};
const int EPD_BUSY[7] = {16, 18, 37, 9, 7, 5, 3};
const int EPD_RESET_MPD[7] = {14, 13, 12, 11, 10, 9, 8};

//const int EPD_CS[7] = {2, 4, 6, 10, 33, 21, 17};
//const int EPD_BUSY[7] = {3, 5, 7, 9, 37, 18, 16};
//const int EPD_RESET_MPD[7] = {8, 9, 10, 11, 12, 13, 14};
// const int EPD_CS[7] = {2, 4, 10, 38, 34, 21, 17};
// const int EPD_BUSY[7] = {3, 5, 9, 36, 35, 18, 16};
const int EPD_DC = 14;
const int RST_PIN = 15;
#elif defined(IS_S2)

// reversed
const int EPD_CS[7] = {17, 21, 33, 10, 6, 4, 2};
const int EPD_BUSY[7] = {16, 18, 37, 9, 7, 5, 3};
const int EPD_RESET_MPD[7] = {14, 13, 12, 11, 10, 9, 8};

// const int EPD_CS[7] = {1, 4, 6, 8, 10, 13, 40};
// const int EPD_BUSY[7] = {3, 5, 7, 9, 11, 12, 39};
const int EPD_DC = 14;
const int RST_PIN = 15;

#elif defined(ARDUINO_ESP32S3_DEV)
const int EPD_CS[7] = {40, 39, 9, 10, 3, 8, 18};
const int EPD_BUSY[7] = {4, 5, 6, 7, 15, 16, 17};
const int EPD_RESET_MPD[7] = {8, 9, 10, 11, 12, 13, 14};

const int EPD_DC = 1;
const int RST_PIN = 2;

#else
const int EPD_CS[7] = {21, 13, 5, 17, 4, 2, 15};
const int EPD_BUSY[7] = {36, 39, 34, 35, 32, 33, 25};

// const int EPD_CS[7] = {4, 14, 5, 17, 16, 4, 15};
// const int EPD_BUSY[7] = {5, 27, 34, 35, 32, 33, 25};
const int EPD_RESET_MPD[7] = {8, 9, 10, 11, 12, 13, 14};

const int EPD_DC = 22;
const int RST_PIN = 12;
#endif

#ifdef IS_BW
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> displays[7] = {
    GxEPD2_213_B74(EPD_CS[0], EPD_DC, /*RST=*/-1, EPD_BUSY[0]),
    GxEPD2_213_B74(EPD_CS[1], EPD_DC, /*RST=*/-1, EPD_BUSY[1]),
    GxEPD2_213_B74(EPD_CS[2], EPD_DC, /*RST=*/-1, EPD_BUSY[2]),
    GxEPD2_213_B74(EPD_CS[3], EPD_DC, /*RST=*/-1, EPD_BUSY[3]),
    GxEPD2_213_B74(EPD_CS[4], EPD_DC, /*RST=*/-1, EPD_BUSY[4]),
    GxEPD2_213_B74(EPD_CS[5], EPD_DC, /*RST=*/-1, EPD_BUSY[5]),
    GxEPD2_213_B74(EPD_CS[6], EPD_DC, /*RST=*/-1, EPD_BUSY[6]),
};

const int SEM_WAIT_TIME = 10000;

#else
GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> displays[7] = {
    GxEPD2_213_Z98c(EPD_CS[0], EPD_DC, /*RST=*/-1, EPD_BUSY[0]),
    GxEPD2_213_Z98c(EPD_CS[1], EPD_DC, /*RST=*/-1, EPD_BUSY[1]),
    GxEPD2_213_Z98c(EPD_CS[2], EPD_DC, /*RST=*/-1, EPD_BUSY[2]),
    GxEPD2_213_Z98c(EPD_CS[3], EPD_DC, /*RST=*/-1, EPD_BUSY[3]),
    GxEPD2_213_Z98c(EPD_CS[4], EPD_DC, /*RST=*/-1, EPD_BUSY[4]),
    GxEPD2_213_Z98c(EPD_CS[5], EPD_DC, /*RST=*/-1, EPD_BUSY[5]),
    GxEPD2_213_Z98c(EPD_CS[6], EPD_DC, /*RST=*/-1, EPD_BUSY[6]),
};

const int SEM_WAIT_TIME = 30000;

#endif

const uint displaySize = 7;
uint32_t lastFullRefresh[displaySize];

std::array<String, 7> currentEpdContent;
std::array<String, 7> epdContent;
TaskHandle_t tasks[displaySize];
SemaphoreHandle_t epdUpdateSemaphore[displaySize];
uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];

void setupDisplays()
{
    handleScreenTasks(getCurrentScreen());
    xTaskCreate(taskEpd, "epd_task", 2048, NULL, 1, NULL);
}

void resetAllDisplays()
{
    digitalWrite(RST_PIN, HIGH);
    pinMode(RST_PIN, OUTPUT);
    delay(20);
    digitalWrite(RST_PIN, LOW);
    delay(20);
    digitalWrite(RST_PIN, HIGH);
    delay(200);

    // for (int i = 8; i < 16; i++) {
    //     mcp.digitalWrite(i, HIGH);
    //     mcp.pinMode(i, OUTPUT);
    //     delay(20);
    //     mcp.digitalWrite(i, LOW);
    //     delay(20);
    //     mcp.digitalWrite(i, HIGH);
    //     delay(200);
    // }
}

void resetSingleDisplay(int i)
{
    #ifndef NO_MCP
    mcp.digitalWrite(EPD_RESET_MPD[i], HIGH);
    delay(20);
    mcp.digitalWrite(EPD_RESET_MPD[i], LOW);
    delay(20);
    mcp.digitalWrite(EPD_RESET_MPD[i], HIGH);
    delay(200);
    #endif
}

void initDisplays()
{
    for (uint i = 0; i < displaySize; i++)
    {
#ifndef NO_MCP
        mcp.pinMode(EPD_RESET_MPD[i], OUTPUT);
#endif
        displays[i].init();
#ifndef NO_MCP
        resetSingleDisplay(i);
#endif

        //  displays[i].epd2.init(SW_SCK, SW_MOSI, 115200, true, 20, false);
    }

    std::string text = "BTClock";
    for (uint i = 0; i < displaySize; i++)
    {
        epdUpdateSemaphore[i] = xSemaphoreCreateBinary();
        xSemaphoreGive(epdUpdateSemaphore[i]);

        // Allocate memory for the task parameter
        int *taskParam = new int;
        *taskParam = i;

        xTaskCreate(updateDisplay, "EpdUpd" + char(i), 2048, taskParam, 1, &tasks[i]); // create task
                                                                                       //  delay(1000);
    }
    epdContent = {"B", "T", "C", "L", "O", "C", "K"};
    for (uint i = 0; i < displaySize; i++)
    {
        xTaskNotifyGive(tasks[i]);
    }
    vTaskDelay(pdMS_TO_TICKS(displays[0].epd2.full_refresh_time));
}

void taskEpd(void *pvParameters)
{
    while (1)
    {
        switch (getCurrentScreen())
        {
        case SCREEN_BLOCK_HEIGHT:
            epdContent = BlockHeightScreen::getEpdContent();
            break;
        case SCREEN_BTC_TICKER:
            epdContent = TickerScreen::getEpdContent();
            break;
        case SCREEN_MSCW_TIME:
            epdContent = SatsPerDollarScreen::getEpdContent();
            break;
        case SCREEN_TIME:
            epdContent = TimeScreen::getEpdContent();
            break;
        case SCREEN_HALVING_COUNTDOWN:
            epdContent = HalvingCountdownScreen::getEpdContent();
            break;
        case SCREEN_COUNTDOWN:
            epdContent = CountdownScreen::getEpdContent();
            break;
        case SCREEN_CUSTOM:
            epdContent = CustomTextScreen::getEpdContent();
            break;
        }


        bool updatedThisCycle = false;


        for (uint i = 0; i < displaySize; i++)
        {
            if (epdContent[i].compareTo(currentEpdContent[i]) != 0)
            {
                if (!updatedThisCycle)
                {
#ifdef NO_MCP
                    resetAllDisplays();
#endif
                    updatedThisCycle = true;
                }

#ifndef NO_DISPLAY

                if (xSemaphoreTake(epdUpdateSemaphore[i], pdMS_TO_TICKS(SEM_WAIT_TIME)) == pdTRUE)
                {
                    xTaskNotifyGive(tasks[i]);
                }
                else
                {
                    Serial.println("Couldnt get screen" + String(i));
                }
#else
                currentEpdContent[i] = epdContent[i];
#endif
            }
        }

        #ifdef WITH_RGB_LED
        if (updatedThisCycle && preferences.getBool("ledFlashOnUpd", false))
        {
            xTaskNotifyGive(ledHandlerTaskHandle);
            Serial.println("Flash leds");
        }
        #endif

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

std::array<String, 7> getCurrentEpdContent()
{
    return currentEpdContent;
}

void setEpdContent(std::array<String, 7> newEpdContent)
{
    epdContent = newEpdContent;
}

void splitText(uint dispNum, String top, String bottom, bool partial)
{
    displays[dispNum].setRotation(2);
    displays[dispNum].setFont(&FONT_SMALL);
    displays[dispNum].setTextColor(getFgColor());

    // Top text
    int16_t ttbx, ttby;
    uint16_t ttbw, ttbh;
    displays[dispNum].getTextBounds(top, 0, 0, &ttbx, &ttby, &ttbw, &ttbh);
    uint16_t tx = ((displays[dispNum].width() - ttbw) / 2) - ttbx;
    uint16_t ty = ((displays[dispNum].height() - ttbh) / 2) - ttby - ttbh / 2 - 12;

    // Bottom text
    int16_t tbbx, tbby;
    uint16_t tbbw, tbbh;
    displays[dispNum].getTextBounds(bottom, 0, 0, &tbbx, &tbby, &tbbw, &tbbh);
    uint16_t bx = ((displays[dispNum].width() - tbbw) / 2) - tbbx;
    uint16_t by = ((displays[dispNum].height() - tbbh) / 2) - tbby + tbbh / 2 + 12;

    // Make separator as wide as the shortest text.
    uint16_t lineWidth, lineX;
    if (tbbw < ttbh)
        lineWidth = tbbw;
    else
        lineWidth = ttbw;
    lineX = round((displays[dispNum].width() - lineWidth) / 2);

    displays[dispNum].fillScreen(getBgColor());
    displays[dispNum].setCursor(tx, ty);
    displays[dispNum].print(top);
    displays[dispNum].fillRoundRect(lineX, displays[dispNum].height() / 2 - 3, lineWidth, 6, 3, getFgColor());
    displays[dispNum].setCursor(bx, by);
    displays[dispNum].print(bottom);
}

void showDigit(uint dispNum, char chr, bool partial, const GFXfont *font)
{
    String str(chr);
    displays[dispNum].setRotation(2);
    displays[dispNum].setFont(font);
    displays[dispNum].setTextColor(getFgColor());
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    displays[dispNum].getTextBounds(str, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center the bounding box by transposition of the origin:
    uint16_t x = ((displays[dispNum].width() - tbw) / 2) - tbx;
    uint16_t y = ((displays[dispNum].height() - tbh) / 2) - tby;
    displays[dispNum].fillScreen(getBgColor());
    displays[dispNum].setCursor(x, y);
    displays[dispNum].print(str);
}

void fullRefresh(void *pvParameters)
{
    resetAllDisplays();

    for (uint i = 0; i < displaySize; i++)
    {
        lastFullRefresh[i] = NULL;
    }

    vTaskDelete(NULL);
}

void updateDisplay(void *pvParameters)
{
    const int epdIndex = *(int *)pvParameters;
    delete (int *)pvParameters;

    for (;;)
    {
        // Wait for the task notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (epdContent[epdIndex].compareTo(currentEpdContent[epdIndex]) != 0)
        {
            currentEpdContent[epdIndex] = epdContent[epdIndex];
// resetSingleDisplay(epdIndex);
#ifndef NO_MCP
            displays[epdIndex].init(0, false);
            delay(displays[epdIndex].epd2.power_on_time);
            resetSingleDisplay(epdIndex);
#endif
            // displays[epdIndex].init(0, false);
            bool updatePartial = true;
            if (!lastFullRefresh[epdIndex])
            {
                updatePartial = false;
                lastFullRefresh[epdIndex] = millis();
            }
#ifndef NO_DISPLAY
            if (strstr(epdContent[epdIndex].c_str(), "/") != NULL)
            {
                String top = epdContent[epdIndex].substring(0, epdContent[epdIndex].indexOf("/"));
                String bottom = epdContent[epdIndex].substring(epdContent[epdIndex].indexOf("/") + 1);
                splitText(epdIndex, top, bottom, updatePartial);
            }
            else
            {
                showDigit(epdIndex, epdContent[epdIndex].c_str()[0], updatePartial, &FONT_BIG);
            }

            displays[epdIndex].display(updatePartial);
            // displays[epdIndex].powerOff();
            displays[epdIndex].hibernate();
#endif
        }
        xSemaphoreGive(epdUpdateSemaphore[epdIndex]);
    }
}

void showSetupQr(String ssid, String password)
{
    int displayIndex = 6;
    
    const String text = "WIFI:S:" + ssid + ";T:WPA;P:" + password + ";;"; // User-supplied text

//    genQrCode(text, &qrcode);
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(text.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_LOW,
                                qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

    const int size = qrcodegen_getSize(qrcode);

    const int padding = floor(float(displays[displayIndex].width() - (size * 4)) / 2);
    const int paddingY = floor(float(displays[displayIndex].height() - (size * 4)) / 2);
    //  displays[displayIndex].setRotation(0);
   // displays[displayIndex].setFullWindow();
    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].firstPage();

    displays[displayIndex].fillScreen(GxEPD_WHITE);
    int border = 0;
    do
    {
        for (int y = -border; y < size * 4 + border; y++)
        {
            for (int x = -border; x < size * 4 + border; x++)
            {
                displays[displayIndex].drawPixel(padding + x, paddingY + y, qrcodegen_getModule(qrcode, floor(float(x) / 4), floor(float(y) / 4)) ? GxEPD_BLACK : GxEPD_WHITE);
            }
        }
    } while (displays[displayIndex].nextPage());

    displayIndex = 4;

    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].firstPage();

    displays[displayIndex].fillScreen(GxEPD_WHITE);
    do
    {
        displays[displayIndex].setTextColor(GxEPD_BLACK);
        displays[displayIndex].setCursor(0, 50);
        displays[displayIndex].setFont(&FreeSansBold9pt7b);
        displays[displayIndex].println("SSID:");
        displays[displayIndex].setFont(&FreeSans9pt7b);
        displays[displayIndex].println(ssid);
        displays[displayIndex].println("");
        displays[displayIndex].setFont(&FreeSansBold9pt7b);
        displays[displayIndex].println("Password:");
        displays[displayIndex].setFont(&FreeSans9pt7b);
        displays[displayIndex].println(password);
    } while (displays[displayIndex].nextPage());

    displayIndex = 2;

    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].firstPage();

    displays[displayIndex].fillScreen(GxEPD_WHITE);
    do
    {
        displays[displayIndex].setTextColor(GxEPD_BLACK);
        displays[displayIndex].setCursor(0, 50);
        displays[displayIndex].setFont(&FreeSans9pt7b);
        displays[displayIndex].println("To setup");
        displays[displayIndex].println("scan QR or");
        displays[displayIndex].println("connect");
        displays[displayIndex].println("manually");
    } while (displays[displayIndex].nextPage());

    displayIndex = 0;

    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].firstPage();

    displays[displayIndex].fillScreen(GxEPD_WHITE);
    do
    {
        displays[displayIndex].setTextColor(GxEPD_BLACK);
        displays[displayIndex].setCursor(0, 50);
        displays[displayIndex].setFont(&FreeSansBold9pt7b);
        displays[displayIndex].println("Welcome!");
    } while (displays[displayIndex].nextPage());

    for (int i = 1; i < displaySize; (i = i+2)) {
        displays[i].setPartialWindow(0, 0, displays[i].width(), displays[i].height());
//        displays[i].firstPage();

        displays[i].fillScreen(GxEPD_WHITE);
        displays[i].display(true);
    }

    for (int i = 0; i < displaySize; i++) {
        displays[i].hibernate();
    }
}
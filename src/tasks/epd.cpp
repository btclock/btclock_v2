#include "epd.hpp"

#ifdef IS_S3

// GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> displays[NUM_SCREENS] = {
//     GxEPD2_213_B74(&Native_Pin(EPD_CS[0]), &Native_Pin(EPD_DC), &MCP23X17_Pin(mcp, EPD_RESET_MPD[0]), &Native_Pin(EPD_BUSY[0])),
//     GxEPD2_213_B74(&Native_Pin(EPD_CS[1]), &Native_Pin(EPD_DC), &MCP23X17_Pin(mcp, EPD_RESET_MPD[1]), &Native_Pin(EPD_BUSY[1])),
//     GxEPD2_213_B74(&Native_Pin(EPD_CS[2]), &Native_Pin(EPD_DC), &MCP23X17_Pin(mcp, EPD_RESET_MPD[2]), &Native_Pin(EPD_BUSY[2])),
//     GxEPD2_213_B74(&Native_Pin(EPD_CS[3]), &Native_Pin(EPD_DC), &MCP23X17_Pin(mcp, EPD_RESET_MPD[3]), &Native_Pin(EPD_BUSY[3])),
//     GxEPD2_213_B74(&Native_Pin(EPD_CS[4]), &Native_Pin(EPD_DC), &MCP23X17_Pin(mcp, EPD_RESET_MPD[4]), &Native_Pin(EPD_BUSY[4])),
//     GxEPD2_213_B74(&Native_Pin(EPD_CS[5]), &Native_Pin(EPD_DC), &MCP23X17_Pin(mcp, EPD_RESET_MPD[5]), &Native_Pin(EPD_BUSY[5])),
//     GxEPD2_213_B74(&Native_Pin(EPD_CS[6]), &Native_Pin(EPD_DC), &MCP23X17_Pin(mcp, EPD_RESET_MPD[6]), &Native_Pin(EPD_BUSY[6])),
// };

Native_Pin EPD_CS[NUM_SCREENS] = {Native_Pin(2), Native_Pin(4), Native_Pin(6), Native_Pin(10), Native_Pin(33), Native_Pin(21), Native_Pin(17)};
Native_Pin EPD_BUSY[NUM_SCREENS] = {Native_Pin(3), Native_Pin(5), Native_Pin(7), Native_Pin(9), Native_Pin(37), Native_Pin(18), Native_Pin(16)};
MCP23X17_Pin EPD_RESET_MPD[NUM_SCREENS] = {MCP23X17_Pin(mcp,8), MCP23X17_Pin(mcp, 9), MCP23X17_Pin(mcp, 10), MCP23X17_Pin(mcp, 11), MCP23X17_Pin(mcp, 12), MCP23X17_Pin(mcp, 13), MCP23X17_Pin(mcp, 14)};

Native_Pin EPD_DC = Native_Pin(14);
//const char RST_PIN = 15;
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
const int RST_PIN = 2;
#endif

#ifdef IS_BW

GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> displays[NUM_SCREENS] = {
    GxEPD2_213_B74(&EPD_CS[0], &EPD_DC, &EPD_RESET_MPD[0], &EPD_BUSY[0]),
    GxEPD2_213_B74(&EPD_CS[1], &EPD_DC, &EPD_RESET_MPD[1], &EPD_BUSY[1]),
    GxEPD2_213_B74(&EPD_CS[2], &EPD_DC, &EPD_RESET_MPD[2], &EPD_BUSY[2]),
    GxEPD2_213_B74(&EPD_CS[3], &EPD_DC, &EPD_RESET_MPD[3], &EPD_BUSY[3]),
    GxEPD2_213_B74(&EPD_CS[4], &EPD_DC, &EPD_RESET_MPD[4], &EPD_BUSY[4]),
    GxEPD2_213_B74(&EPD_CS[5], &EPD_DC, &EPD_RESET_MPD[5], &EPD_BUSY[5]),
    GxEPD2_213_B74(&EPD_CS[6], &EPD_DC, &EPD_RESET_MPD[6], &EPD_BUSY[6]),
};

// GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> * displays2 = (GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> *) ps_malloc(7 * sizeof (GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT>));

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

uint32_t lastFullRefresh[NUM_SCREENS];

std::array<String, 7> currentEpdContent;
std::array<String, 7> epdContent;
TaskHandle_t tasks[NUM_SCREENS];
SemaphoreHandle_t epdUpdateSemaphore[NUM_SCREENS];
//

//int *qrcode = (int *) ps_malloc(qrcodegen_BUFFER_LEN_MAX * sizeof(uint8_t));


void setupDisplays()
{
    handleScreenTasks(getCurrentScreen());
    xTaskCreate(taskEpd, "epd_task", 2048, NULL, 1, NULL);
}

void resetAllDisplays()
{
#ifdef NO_MCP
    digitalWrite(RST_PIN, HIGH);
    pinMode(RST_PIN, OUTPUT);
    delay(20);
    digitalWrite(RST_PIN, LOW);
    delay(20);
    digitalWrite(RST_PIN, HIGH);
    delay(200);
#else
    for (int i = 0; i < NUM_SCREENS; i++)
    {
        resetSingleDisplay(i);
    }
#endif
}

void resetSingleDisplay(int i)
{
#ifndef NO_MCP
    EPD_RESET_MPD[i].digitalWrite(HIGH);
    delay(20);
    EPD_RESET_MPD[i].digitalWrite(LOW);
    delay(20);
    EPD_RESET_MPD[i].digitalWrite(HIGH);
    delay(200);
#endif
}

void initDisplays()
{
    for (uint i = 0; i < NUM_SCREENS; i++)
    {
#ifndef NO_MCP
        EPD_RESET_MPD[i].pinMode(OUTPUT);
#endif
        displays[i].init();
#ifndef NO_MCP
        resetSingleDisplay(i);
#endif

        //  displays[i].epd2.init(SW_SCK, SW_MOSI, 115200, true, 20, false);
    }

    for (uint i = 0; i < NUM_SCREENS; i++)
    {
        epdUpdateSemaphore[i] = xSemaphoreCreateBinary();
        xSemaphoreGive(epdUpdateSemaphore[i]);

        // Allocate memory for the task parameter
        int *taskParam = new int;
        *taskParam = i;

        xTaskCreate(updateDisplay, "EpdUpd" + char(i), 4096, taskParam, 1, &tasks[i]); // create task
                                                                                       //  delay(1000);
    }
    epdContent = {"B", "T", "C", "L", "O", "C", "K"};
    for (uint i = 0; i < NUM_SCREENS; i++)
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
            epdContent = TickerScreen::getEpdContentSats();
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

        for (uint i = 0; i < NUM_SCREENS; i++)
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

void splitText(const uint dispNum, String top, String bottom, bool partial)
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

void showDigit(const uint dispNum, char chr, bool partial, const GFXfont *font)
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

    for (uint i = 0; i < NUM_SCREENS; i++)
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
#ifndef NO_MCP
            displays[epdIndex].init(0, false);
            resetSingleDisplay(epdIndex);
#endif
            // displays[epdIndex].init(0, false);
            bool updatePartial = true;

            // Full Refresh every half hour
            if (!lastFullRefresh[epdIndex] || (millis() - lastFullRefresh[epdIndex]) > (preferences.getUInt("fullRefreshMin", 30) * 60 * 1000))
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
            displays[epdIndex].hibernate();
#endif
        }
        xSemaphoreGive(epdUpdateSemaphore[epdIndex]);
    }
}


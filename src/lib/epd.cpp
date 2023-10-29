#include "epd.hpp"

int fgColor;
int bgColor;
uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];

int getBgColor()
{
    return bgColor;
}

int getFgColor()
{
    return fgColor;
}

void setBgColor(int color)
{
    bgColor = color;
}

void setFgColor(int color)
{
    fgColor = color;
}

void showSetupQr(const String &ssid, const String &password)
{
    char displayIndex = 6;

    const String text = "WIFI:S:" + ssid + ";T:WPA;P:" + password + ";;";

    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(text.c_str(), tempBuffer, qrcode, qrcodegen_Ecc_LOW,
                                   qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);

    const int size = qrcodegen_getSize(qrcode);

    const int padding = floor(float(displays[displayIndex].width() - (size * 4)) / 2);
    const int paddingY = floor(float(displays[displayIndex].height() - (size * 4)) / 2);

    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].fillScreen(GxEPD_WHITE);
    const int border = 0;

    for (int y = -border; y < size * 4 + border; y++)
    {
        for (int x = -border; x < size * 4 + border; x++)
        {
            displays[displayIndex].drawPixel(padding + x, paddingY + y, qrcodegen_getModule(qrcode, floor(float(x) / 4), floor(float(y) / 4)) ? GxEPD_BLACK : GxEPD_WHITE);
        }
    }
    displays[displayIndex].display(true);

    displayIndex = 4;

    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].fillScreen(GxEPD_WHITE);
    displays[displayIndex].setTextColor(GxEPD_BLACK);
    displays[displayIndex].setCursor(0, 50);
    displays[displayIndex].setFont(&FreeSansBold9pt7b);
    displays[displayIndex].println(F("SSID:"));
    displays[displayIndex].setFont(&FreeSans9pt7b);
    displays[displayIndex].println(ssid);
    displays[displayIndex].println("");
    displays[displayIndex].setFont(&FreeSansBold9pt7b);
    displays[displayIndex].println(F("Password:"));
    displays[displayIndex].setFont(&FreeSans9pt7b);
    displays[displayIndex].println(password);
    displays[displayIndex].display(true);

    displayIndex = 2;
    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].fillScreen(GxEPD_WHITE);
    displays[displayIndex].setTextColor(GxEPD_BLACK);
    displays[displayIndex].setCursor(0, 50);
    displays[displayIndex].setFont(&FreeSans9pt7b);
    displays[displayIndex].println(F("To setup\r\nscan QR or\r\nconnect\r\nmanually"));
    // displays[displayIndex].println(F("scan QR or"));
    // displays[displayIndex].println(F("connect"));
    // displays[displayIndex].println(F("manually"));
    displays[displayIndex].display(true);

    displayIndex = 0;

    displays[displayIndex].setPartialWindow(0, 0, displays[displayIndex].width(), displays[displayIndex].height());
    displays[displayIndex].fillScreen(GxEPD_WHITE);
    displays[displayIndex].setTextColor(GxEPD_BLACK);
    displays[displayIndex].setCursor(0, 50);
    displays[displayIndex].setFont(&FreeSansBold9pt7b);
    displays[displayIndex].println(F("Welcome!"));
    displays[displayIndex].display(true);

    for (int i = 1; i < NUM_SCREENS; (i = i + 2))
    {
        displays[i].setPartialWindow(0, 0, displays[i].width(), displays[i].height());
        displays[i].fillScreen(GxEPD_WHITE);
        displays[i].display(true);
    }

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        displays[i].hibernate();
    }
}
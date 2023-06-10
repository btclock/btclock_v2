#include "sats_per_dollar.hpp"

uint SatsPerDollarScreen::satsPerDollar = 0;
std::array<String, 7> SatsPerDollarScreen::epdContent = { "", "", "", "", "", "", "" };

void SatsPerDollarScreen::init() {
    SatsPerDollarScreen::satsPerDollar = int(round(1 / preferences.getFloat("btcPrice", 12345) * 10e7));
    setupGetPriceTask();
    SatsPerDollarScreen::showScreen();
}

void SatsPerDollarScreen::showScreen() {
    std::string satsPerDollarString = String(SatsPerDollarScreen::satsPerDollar).c_str();
    satsPerDollarString.insert(satsPerDollarString.begin(), 7 - satsPerDollarString.length(), ' ');
    epdContent[0] = "MSCW/TIME";
    for (uint i = 1; i < 7; i++)
    {
        SatsPerDollarScreen::epdContent[i] = satsPerDollarString[i];
    }
}

void SatsPerDollarScreen::onPriceUpdate(uint price) {
    SatsPerDollarScreen::satsPerDollar = int(round(1 / float(price) * 10e7));

    SatsPerDollarScreen::showScreen();
}

std::array<String, 7> SatsPerDollarScreen::getEpdContent() {
    return SatsPerDollarScreen::epdContent;
}
#include "ticker.hpp"

uint TickerScreen::price = 12345;
uint TickerScreen::satsPerDollar = 3000;

std::array<String, NUM_SCREENS> TickerScreen::epdContent = { "", "", "", "", "", "", "" };

void TickerScreen::init() {

    TickerScreen::price = preferences.getFloat("btcPrice", 12345);;
    TickerScreen::satsPerDollar = int(round(1 / preferences.getFloat("btcPrice", 12345) * 10e7));

    setupGetPriceTask();
    TickerScreen::showScreen();
}

void TickerScreen::showScreen() {
    std::string priceString = ("$" + String(TickerScreen::price)).c_str();
    priceString.insert(priceString.begin(), NUM_SCREENS - priceString.length(), ' ');
    epdContent[0] = "BTC/USD";
    for (uint i = 1; i < NUM_SCREENS; i++)
    {
        TickerScreen::epdContent[i] = priceString[i];
    }
}

void TickerScreen::onPriceUpdate(uint price) {
    TickerScreen::price = price;
    TickerScreen::satsPerDollar = int(round(1 / float(price) * 10e7));

    TickerScreen::showScreen();
}

std::array<String, NUM_SCREENS> TickerScreen::getEpdContent() {
    return TickerScreen::epdContent;
}

std::array<String, NUM_SCREENS> TickerScreen::getEpdContentSats() {
    std::array<String, NUM_SCREENS> epdContentSats = { "", "", "", "", "", "", "" };

    std::string satsPerDollarString = String(TickerScreen::satsPerDollar).c_str();
    satsPerDollarString.insert(satsPerDollarString.begin(), NUM_SCREENS - satsPerDollarString.length(), ' ');
    epdContentSats[0] = "MSCW/TIME";
    for (uint i = 1; i < NUM_SCREENS; i++)
    {
        epdContentSats[i] = satsPerDollarString[i];
    }


    return epdContentSats;
}

uint TickerScreen::getPrice() {
    return price;
}
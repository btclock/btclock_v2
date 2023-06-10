#include "ticker.hpp"

uint TickerScreen::price = 12345;
std::array<String, 7> TickerScreen::epdContent = { "", "", "", "", "", "", "" };

void TickerScreen::init() {
    TickerScreen::price = preferences.getFloat("btcPrice", 12345);;
    setupGetPriceTask();
    TickerScreen::showScreen();
}

void TickerScreen::showScreen() {
    std::string priceString = ("$" + String(TickerScreen::price)).c_str();
    priceString.insert(priceString.begin(), 7 - priceString.length(), ' ');
    epdContent[0] = "BTC/USD";
    for (uint i = 1; i < 7; i++)
    {
        TickerScreen::epdContent[i] = priceString[i];
    }
}

void TickerScreen::onPriceUpdate(uint price) {
    TickerScreen::price = price;
    TickerScreen::showScreen();
}

std::array<String, 7> TickerScreen::getEpdContent() {
    return TickerScreen::epdContent;
}
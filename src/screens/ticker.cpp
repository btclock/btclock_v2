#include "ticker.hpp"

uint TickerScreen::price = 12345;
std::array<String, NUM_SCREENS> TickerScreen::epdContent = { "", "", "", "", "", "", "" };

void TickerScreen::init() {
    TickerScreen::price = preferences.getFloat("btcPrice", 12345);;
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
    TickerScreen::showScreen();
}

std::array<String, NUM_SCREENS> TickerScreen::getEpdContent() {
    return TickerScreen::epdContent;
}
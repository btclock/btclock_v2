#include "market_cap.hpp"

uint MarketCapScreen::satsPerDollar = 0;
std::array<String, NUM_SCREENS> MarketCapScreen::epdContent = {"", "", "", "", "", "", ""};

void MarketCapScreen::init()
{
    // Dependent on price and blocks
    MarketCapScreen::showScreen();
}

void MarketCapScreen::showScreen()
{
    double supply = getSupplyAtBlock(BlockHeightScreen::getBlockNr());
    int64_t marketCap = static_cast<std::int64_t>(supply * double(TickerScreen::getPrice()));

    std::string priceString = "$" + formatNumberWithSuffix(marketCap);
    priceString.insert(priceString.begin(), NUM_SCREENS - priceString.length(), ' ');

    epdContent[0] = "USD/MCAP";
    for (uint i = 1; i < NUM_SCREENS; i++)
    {
        MarketCapScreen::epdContent[i] = priceString[i];
    }
}

std::array<String, NUM_SCREENS> MarketCapScreen::getEpdContent()
{
    return MarketCapScreen::epdContent;
}
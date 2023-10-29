#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include "tasks/epd.hpp"

class TickerScreen
{
protected:
    static uint price;
    static uint satsPerDollar;

    static std::array<String, NUM_SCREENS> epdContent;
public:
    static void init();
    static void showScreen();
    static void onPriceUpdate(uint price);
    static std::array<String, NUM_SCREENS> getEpdContent();
    static std::array<String, NUM_SCREENS> getEpdContentSats();
};
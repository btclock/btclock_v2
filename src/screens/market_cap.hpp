#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include "lib/utils.hpp"
#include "blockheight.hpp";
#include "ticker.hpp";
#include "tasks/epd.hpp"

class MarketCapScreen {
    protected:
        static uint satsPerDollar;
        static std::array<String, NUM_SCREENS> epdContent;
    public:
        static void init();
        static void showScreen();
        static void onPriceUpdate(uint price);
        static std::array<String, NUM_SCREENS> getEpdContent();
};
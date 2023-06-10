#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include "tasks/epd.hpp"
#include "tasks/blocknotify.hpp"

class HalvingCountdownScreen {
    protected:
        static  uint currentBlockNr;
        static  uint halvingBlockNr;
        static std::array<String, 7> epdContent;
    public:
        static void init();
        static void showScreen();
        static void onNewBlock(uint blockNr);
        static uint getNextHalvingBlockNr();
        static std::array<String, 7> getEpdContent();
};
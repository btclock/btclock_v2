#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include "tasks/epd.hpp"
#include "tasks/blocknotify.hpp"

class BlockHeightScreen
{
protected:
    static uint blockNr;
    static char** epdContent;
    static char* psramBuffer;
public:
    static void init();
    static void showScreen();
    static void onNewBlock(uint blockNr);
    static std::array<String, NUM_SCREENS> getEpdContent();
    static uint getBlockNr();
};
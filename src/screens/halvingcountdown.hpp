#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include "tasks/epd.hpp"
#include "tasks/blocknotify.hpp"

class HalvingCountdownScreen
{
protected:
    static uint currentBlockNr;
    static uint halvingBlockNr;
    //static std::array<char*, NUM_SCREENS> epdContent;
   // static char epdContent[NUM_SCREENS][12];
    static char** epdContentP;
//    static char **epdContent;
    static char *psramBuffer;

public:
    static void init();
    static void showScreen();
    static void onNewBlock(uint blockNr);
    static uint getNextHalvingBlockNr();
    static std::array<String, NUM_SCREENS> getEpdContent();
};
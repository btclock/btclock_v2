#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include "tasks/epd.hpp"

class TimeScreen {
    protected:
        static std::array<String, NUM_SCREENS> epdContent;
        static TimeScreen* instance_;
    public:
        static void init();
        static void showScreen();
        static void onNewMinute();
        static void onActivate();
        static std::array<String, NUM_SCREENS> getEpdContent();
        static TimeScreen* getInstance();
};
#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include <string>
#include <iostream>
#include "tasks/epd.hpp"

class CountdownScreen {
    protected:
        static uint countdownMinutes;
        static uint countdownSeconds;
        static std::array<String, NUM_SCREENS> epdContent;
    public:
        static void init();
        static void showScreen();
        static std::array<String, NUM_SCREENS> getEpdContent();
        static void setCountdownSeconds(uint sec);
        static void countdownTask(void *pvParameters);
};
#pragma once

#include "base.hpp"
#include "config.h"
#include "shared.hpp"
#include <string>
#include <iostream>
#include "tasks/epd.hpp"

class CustomTextScreen {
    protected:
        static std::string customText;
        static std::array<String, NUM_SCREENS> epdContent;
    public:
        static void init();
        static void showScreen();
        static std::array<String, NUM_SCREENS> getEpdContent();
        static void setSimpleText(const String& text);
        static void setText(std::array<String, NUM_SCREENS> customContent);
};
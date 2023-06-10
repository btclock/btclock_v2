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
        static std::array<String, 7> epdContent;
    public:
        static void init();
        static void showScreen();
        static std::array<String, 7> getEpdContent();
        static void setSimpleText(String text);
        static void setText(std::array<String, 7> customContent);
};
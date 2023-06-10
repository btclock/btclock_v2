#pragma once
#include "config.h"
#include "shared.hpp"

class BcScreen
{
    protected:
        std::array<String, 7> epdContent;
     public:
        void init();
        void showScreen();
        std::array<String, 7> getEpdContent();
};
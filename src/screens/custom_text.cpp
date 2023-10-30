#include "custom_text.hpp"

std::string CustomTextScreen::customText = "";
std::array<String, NUM_SCREENS> CustomTextScreen::epdContent = {"", "", "", "", "", "", ""};

void CustomTextScreen::init()
{
    CustomTextScreen::showScreen();
}

void CustomTextScreen::showScreen()
{
}

void CustomTextScreen::setSimpleText(const String& text)
{
    customText = text.c_str();

    customText.insert(customText.begin(), NUM_SCREENS - customText.length(), ' ');

    for (uint i = 0; i < NUM_SCREENS; i++)
    {
        CustomTextScreen::epdContent[i] = customText[i];
    }
}

void CustomTextScreen::setText(std::array<String, NUM_SCREENS> customContent)
{
    epdContent = customContent;
}


std::array<String, NUM_SCREENS> CustomTextScreen::getEpdContent()
{
    return CustomTextScreen::epdContent;
}
#include "custom_text.hpp"

std::string CustomTextScreen::customText = "";
std::array<String, 7> CustomTextScreen::epdContent = {"", "", "", "", "", "", ""};

void CustomTextScreen::init()
{
    CustomTextScreen::showScreen();
}

void CustomTextScreen::showScreen()
{
}

void CustomTextScreen::setSimpleText(String text)
{
    customText = text.c_str();

    customText.insert(customText.begin(), 7 - customText.length(), ' ');

    for (uint i = 0; i < 7; i++)
    {
        CustomTextScreen::epdContent[i] = customText[i];
    }
}

void CustomTextScreen::setText(std::array<String, 7> customContent)
{
    epdContent = customContent;
}


std::array<String, 7> CustomTextScreen::getEpdContent()
{
    return CustomTextScreen::epdContent;
}
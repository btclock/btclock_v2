#include "time.hpp"

String TimeScreen::timeString = "";
String TimeScreen::dateString = "";
std::array<String, 7> TimeScreen::epdContent = { "", "", "", "", "", "", "" };

void TimeScreen::init() { 
    setupMinuteEvent();
    TimeScreen::showScreen();
}

void TimeScreen::showScreen() {
    TimeScreen::dateString = String(rtc.getDay()) + "/" + String(rtc.getMonth() + 1);
    TimeScreen::timeString = rtc.getTime("%H:%M").c_str();

    std::string timeString = TimeScreen::timeString.c_str();
    timeString.insert(timeString.begin(), 7 - timeString.length(), ' ');
    TimeScreen::epdContent[0] = TimeScreen::dateString;
    for (uint i = 1; i < 7; i++)
    {
        TimeScreen::epdContent[i] = timeString[i];
    }
}

void TimeScreen::onNewMinute() {
    TimeScreen::showScreen();
}

void TimeScreen::onActivate() {
    TimeScreen::showScreen();
}

std::array<String, 7> TimeScreen::getEpdContent() {
    return TimeScreen::epdContent;
}

TimeScreen* TimeScreen::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new TimeScreen;
    }

    return instance_;
}
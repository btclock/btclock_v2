#include "time.hpp"

std::array<String, NUM_SCREENS> TimeScreen::epdContent = { "", "", "", "", "", "", "" };

void TimeScreen::init() { 
    setupMinuteEvent();
    TimeScreen::showScreen();
}

void TimeScreen::showScreen() {
    std::string timeString = rtc.getTime("%H:%M").c_str();
    timeString.insert(timeString.begin(), NUM_SCREENS - timeString.length(), ' ');
    char dateTmp[6];
    snprintf(dateTmp, 6, "%d/%d", rtc.getDay(), (rtc.getMonth() + 1));

    TimeScreen::epdContent[0] = dateTmp;

    for (uint i = 1; i < NUM_SCREENS; i++)
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

std::array<String, NUM_SCREENS> TimeScreen::getEpdContent() {
    return TimeScreen::epdContent;
}

TimeScreen* TimeScreen::getInstance() {
    if (instance_ == nullptr) {
        instance_ = new TimeScreen;
    }

    return instance_;
}
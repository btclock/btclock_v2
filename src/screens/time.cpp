#include "time.hpp"

std::array<String, NUM_SCREENS> TimeScreen::epdContent = { "", "", "", "", "", "", "" };

void TimeScreen::init() { 
    setupMinuteEvent();
    TimeScreen::showScreen();
}

void TimeScreen::showScreen() {
   // String(String(rtc.getDay()) + "/" + String(rtc.getMonth() + 1)).toCharArray(TimeScreen::dateString, 5);
  //  rtc.getTime("%H:%M").toCharArray(TimeScreen::timeString, 5);

    std::string timeString = rtc.getTime("%H:%M").c_str();
    timeString.insert(timeString.begin(), NUM_SCREENS - timeString.length(), ' ');
    TimeScreen::epdContent[0] = String(rtc.getDay()) + "/" + String(rtc.getMonth() + 1);
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
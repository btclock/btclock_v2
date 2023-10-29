#include "countdown.hpp"

uint CountdownScreen::countdownMinutes = 1;
uint CountdownScreen::countdownSeconds = 0;
std::array<String, NUM_SCREENS> CountdownScreen::epdContent = {"COUNT/DOWN", "", "", "", "", "", ""};

void CountdownScreen::init()
{
    CountdownScreen::showScreen();
}

void CountdownScreen::showScreen()
{

}

std::array<String, NUM_SCREENS> CountdownScreen::getEpdContent()
{
    return CountdownScreen::epdContent;
}

void CountdownScreen::setCountdownSeconds(uint sec) {
    countdownSeconds = sec;
}

void CountdownScreen::countdownTask(void *pvParameters)
{
    for (int i = CountdownScreen::countdownSeconds; i >= 0; i--)
    {
        char countdownString[NUM_SCREENS];
        sprintf(countdownString, "%02d:%02d", i / 60, i % 60);
        std::string timeString = countdownString;
        timeString.insert(timeString.begin(), NUM_SCREENS - timeString.length(), ' ');
        CountdownScreen::epdContent[0] = "COUNT/DOWN";
        for (uint i = 1; i < 7; i++)
        {
            CountdownScreen::epdContent[i] = timeString[i];
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    Serial.println(F("Countdown finished!"));
    vTaskDelete(NULL);
}
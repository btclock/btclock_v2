#include "halvingcountdown.hpp"

uint HalvingCountdownScreen::currentBlockNr = 0;
uint HalvingCountdownScreen::halvingBlockNr = 0;

std::array<String, 7> HalvingCountdownScreen::epdContent = {"", "", "", "", "", "", ""};

void HalvingCountdownScreen::init()
{
    HalvingCountdownScreen::currentBlockNr = preferences.getUInt("blockHeight", 789000);

    setupBlockNotify();
    HalvingCountdownScreen::showScreen();
}

void HalvingCountdownScreen::showScreen()
{
    uint minutesToHalving = HalvingCountdownScreen::getNextHalvingBlockNr() * 10;

    int years = floor(minutesToHalving / 525600);
    int days = floor((minutesToHalving - (years * 525600)) / (24*60));
    int hours = floor((minutesToHalving - (years * 525600) - (days * (24*60))) / 60);
    int mins = floor(minutesToHalving - (years * 525600) - (days * (24*60)) - (hours * 60));
//    int secs = floor((minutesToHalving - (years * 525600) - (days * (24*60)) - (hours * 60) - mins) * 60);

    epdContent[0] = "BIT/COIN";
    epdContent[1] = "HALV/ING";
    epdContent[2] = String(years) + "/YRS";
    epdContent[3] = String(days) + "/DAYS";
    epdContent[4] = String(hours) + "/HRS";
    epdContent[5] = String(mins) + "/MINS";
    epdContent[6] = "TO/GO";
}

uint HalvingCountdownScreen::getNextHalvingBlockNr()
{
    return 210000 - (HalvingCountdownScreen::currentBlockNr % 210000);
}

void HalvingCountdownScreen::onNewBlock(uint blockNr)
{
    HalvingCountdownScreen::currentBlockNr = blockNr;

    HalvingCountdownScreen::showScreen();
}

std::array<String, 7> HalvingCountdownScreen::getEpdContent()
{
    return HalvingCountdownScreen::epdContent;
}
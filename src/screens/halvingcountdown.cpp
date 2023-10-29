#include "halvingcountdown.hpp"

uint HalvingCountdownScreen::currentBlockNr = 0;
uint HalvingCountdownScreen::halvingBlockNr = 0;

char **HalvingCountdownScreen::epdContentP;
const int maxStringLength = 12;
char *HalvingCountdownScreen::psramBuffer;

//char HalvingCountdownScreen::epdContent[NUM_SCREENS][maxStringLength];
bool initialized = false;

void HalvingCountdownScreen::init()
{
    HalvingCountdownScreen::currentBlockNr = preferences.getUInt("blockHeight", 789000);

    HalvingCountdownScreen::psramBuffer = (char *)ps_malloc(NUM_SCREENS * maxStringLength);

    if (HalvingCountdownScreen::psramBuffer == nullptr)
    {
        Serial.println(F("Failed to allocate memory in PSRAM (HalvingCountdownScreen)"));
    }
    HalvingCountdownScreen::epdContentP = new char*[NUM_SCREENS];

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        epdContentP[i] = HalvingCountdownScreen::psramBuffer + i * maxStringLength;
      //  strcpy(epdContent[i], "x");
        strcpy(epdContentP[i], "x");
    }
    initialized = true;
    setupBlockNotify();
    HalvingCountdownScreen::showScreen();
}

void HalvingCountdownScreen::showScreen()
{
    if (!initialized)
        return;

    uint minutesToHalving = HalvingCountdownScreen::getNextHalvingBlockNr() * 10;

    const int years = floor(minutesToHalving / 525600);
    const int days = floor((minutesToHalving - (years * 525600)) / (24 * 60));
    const int hours = floor((minutesToHalving - (years * 525600) - (days * (24 * 60))) / 60);
    const int mins = floor(minutesToHalving - (years * 525600) - (days * (24 * 60)) - (hours * 60));
    //    int secs = floor((minutesToHalving - (years * 525600) - (days * (24*60)) - (hours * 60) - mins) * 60);

    snprintf(HalvingCountdownScreen::epdContentP[0], maxStringLength, "BIT/COIN");
    snprintf(HalvingCountdownScreen::epdContentP[1], maxStringLength, "HALV/ING");
    snprintf(HalvingCountdownScreen::epdContentP[2], maxStringLength, "%d/YRS", years);

    snprintf(HalvingCountdownScreen::epdContentP[3], maxStringLength, "%d/DAYS", days);
    snprintf(HalvingCountdownScreen::epdContentP[4], maxStringLength, "%d/HRS", hours);
    snprintf(HalvingCountdownScreen::epdContentP[5], maxStringLength, "%d/MINS", mins);
    snprintf(HalvingCountdownScreen::epdContentP[6], maxStringLength, "TO/GO");

    //     // strcpy(epdContent[2], sprintf(String(years) + "/YRS").c_str());
    //    // snprintf(epdContent[2], sizeof(epdContent[2]), "%d/YRS", years);
    //     //  strcpy(epdContent[3], String(days) + "/DAYS");
    //     //    strcpy(epdContent[4], String(hours) + "/HRS");
    //     //  strcpy(epdContent[5], String(mins) + "/MINS");
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

std::array<String, NUM_SCREENS> HalvingCountdownScreen::getEpdContent()
{
    std::array<String, NUM_SCREENS> ret;

    if (!initialized) return ret;

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        ret[i] = HalvingCountdownScreen::epdContentP[i];
    }

    return ret;
}
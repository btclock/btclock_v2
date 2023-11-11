#include "blockheight.hpp"

uint BlockHeightScreen::blockNr = 0;
char **BlockHeightScreen::epdContent;
const int maxStringLength = 15;
char*  BlockHeightScreen::psramBuffer;

void BlockHeightScreen::init()
{
   BlockHeightScreen::psramBuffer = (char*)ps_malloc(NUM_SCREENS * maxStringLength);

    if (BlockHeightScreen::psramBuffer == nullptr)
    {
        Serial.println(F("Failed to allocate memory in PSRAM"));
    }
    BlockHeightScreen::epdContent = new char*[NUM_SCREENS];
    for (int i = 0; i < NUM_SCREENS; i++)
    {
        epdContent[i] = psramBuffer + i * maxStringLength;
    }
    BlockHeightScreen::blockNr = preferences.getUInt("blockHeight", 789000);
    setupBlockNotify();
    BlockHeightScreen::showScreen();
}

void BlockHeightScreen::showScreen()
{
    std::string blockNrString = String(BlockHeightScreen::blockNr).c_str();
    blockNrString.insert(blockNrString.begin(), NUM_SCREENS - blockNrString.length(), ' ');
    //    epdContent[0] = "BLOCK/HEIGHT";
    snprintf(BlockHeightScreen::epdContent[0], 13, "BLOCK/HEIGHT", 0);

    for (uint i = 1; i < NUM_SCREENS; i++)
    {
        snprintf(BlockHeightScreen::epdContent[i], 2, &blockNrString[i], 0);

        //        BlockHeightScreen::epdContent[i] = blockNrString[i];
    }
}

void BlockHeightScreen::onNewBlock(uint blockNr)
{
    BlockHeightScreen::blockNr = blockNr;

    BlockHeightScreen::showScreen();
}

std::array<String, NUM_SCREENS> BlockHeightScreen::getEpdContent()
{
    std::array<String, NUM_SCREENS> ret;

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        ret[i] = BlockHeightScreen::epdContent[i];
    }

    //    std::copy(std::begin(BlockHeightScreen::epdContent), std::end(BlockHeightScreen::epdContent), std::begin(ret));

    return ret;
}

uint BlockHeightScreen::getBlockNr() {
    return blockNr;
}
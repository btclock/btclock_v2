#include "blockheight.hpp"

uint BlockHeightScreen::blockNr = 0;
std::array<String, NUM_SCREENS> BlockHeightScreen::epdContent = { "", "", "", "", "", "", "" };

//std::array<String, NUM_SCREENS> * BlockHeightScreen::epdContent = (std::array<String, NUM_SCREENS> * ) ps_malloc(7 * sizeof (std::array<String, NUM_SCREENS>));


void BlockHeightScreen::init()
{
    BlockHeightScreen::blockNr = preferences.getUInt("blockHeight", 789000);
    setupBlockNotify();
    BlockHeightScreen::showScreen();        
}

void BlockHeightScreen::showScreen()
{
    std::string blockNrString = String(BlockHeightScreen::blockNr).c_str();
    blockNrString.insert(blockNrString.begin(), NUM_SCREENS - blockNrString.length(), ' ');
    epdContent[0] = "BLOCK/HEIGHT";
    for (uint i = 1; i < NUM_SCREENS; i++)
    {
        BlockHeightScreen::epdContent[i] = blockNrString[i];
    }
}

void BlockHeightScreen::onNewBlock(uint blockNr)
{
    BlockHeightScreen::blockNr = blockNr;

    BlockHeightScreen::showScreen();
}

std::array<String, 7> BlockHeightScreen::getEpdContent() {
    return BlockHeightScreen::epdContent;
}
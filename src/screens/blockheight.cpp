#include "blockheight.hpp"

uint BlockHeightScreen::blockNr = 0;
std::array<String, 7> BlockHeightScreen::epdContent = { "", "", "", "", "", "", "" };

void BlockHeightScreen::init()
{
    BlockHeightScreen::blockNr = preferences.getUInt("blockHeight", 789000);
    setupBlockNotify();
    BlockHeightScreen::showScreen();        
}

void BlockHeightScreen::showScreen()
{
    std::string blockNrString = String(BlockHeightScreen::blockNr).c_str();
    blockNrString.insert(blockNrString.begin(), 7 - blockNrString.length(), ' ');
    epdContent[0] = "BLOCK/HEIGHT";
    for (uint i = 1; i < 7; i++)
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
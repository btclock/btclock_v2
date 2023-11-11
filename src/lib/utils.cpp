#include "utils.hpp";

double getSupplyAtBlock(uint blockNr) {
    if (blockNr >= 33 * 210000) {
        return 20999999.9769;
    } 

    const int initialBlockReward = 50;  // Initial block reward
    const int halvingInterval = 210000;  // Number of blocks before halving

    int halvingCount = blockNr / halvingInterval;
    double totalBitcoinInCirculation = 0;

    for (int i = 0; i < halvingCount; ++i) {
        totalBitcoinInCirculation += halvingInterval * initialBlockReward * std::pow(0.5, i);
    }

    totalBitcoinInCirculation += (blockNr % halvingInterval) * initialBlockReward * std::pow(0.5, halvingCount);

    return totalBitcoinInCirculation;
}

std::string formatNumberWithSuffix(int64_t num) {
    const long long quadrillion = 1000000000000000LL;
    const long long trillion = 1000000000000LL;
    const long long billion = 1000000000;
    const long long million = 1000000;
    const long long thousand = 1000;

    if (num >= quadrillion) {
        return std::to_string(num / quadrillion) + "Q";
    } else if (num >= trillion) {
        return std::to_string(num / trillion) + "T";
    } else if (num >= billion) {
        return std::to_string(num / billion) + "B";
    } else if (num >= million) {
        return std::to_string(num / million) + "M";
    } else if (num >= thousand) {
        return std::to_string(num / thousand) + "K";
    } else {
        return std::to_string(num);
    }
}

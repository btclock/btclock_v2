#pragma once

#define NTP_SERVER "pool.ntp.org"
#define DEFAULT_MEMPOOL_INSTANCE "mempool.bitcoin.nl"

#define FONT_SMALL Antonio_SemiBold20pt7b
#define FONT_BIG Antonio_SemiBold90pt7b

#define TIME_OFFSET_SECONDS 3600
#define OTA_PASSWORD "changeme"

#ifndef HOSTNAME
#define HOSTNAME "btclock"
#endif

#define BITCOIND_HOST ""
#define BITCOIND_PORT 8332

#define BITCOIND_RPC_USER ""
#define BITCOIND_RPC_PASS ""

#define DEFAULT_FG_COLOR GxEPD_WHITE
#define DEFAULT_BG_COLOR GxEPD_BLACK

#define USER_AGENT "BTClock/1.0"

#define NUM_SCREENS 7

#define I2C_DEV_ADDR 0x55